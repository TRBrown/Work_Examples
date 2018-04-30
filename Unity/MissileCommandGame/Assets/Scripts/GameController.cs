using UnityEngine;
using UnityEngine.UI;
using System.Collections;
using System.Collections.Generic;

public class GameController : MonoBehaviour {

    public enum gameStates { MenuMode, GameActive, GameOver };

    public GameObject targetZone;
    public GameObject spawnZone;

    public AsteroidControl[] asteroidTypes;
    public float startWait;
    public float spawnWait;
    public float hazardCount;
    public float spawnIncrement;
    public float spawnIncrementTime;
    public float waveWait;
    public float asteroid0Chance;
    public float asteroid1Chance;
    public float asteroid1Multiplier;
    public float asteroid2Chance;
    public float asteroid2Multiplier;
    public int onTargetAsteroids;
    public int onTargetAsteroidsIncrement;

    private float hazardCountStore;
    private float asteroid0ChanceStore;
    private float asteroid1ChanceStore;
    private float asteroid2ChanceStore;
    private int onTargetAsteroidsStore;

    private float gameStartTime;
    private float currentSpawnIncrementTime;
    private float[] probabilityArray;
    
    public GameObject[] missileLaunchers;
    private ObjectPool asteroidPool;
    private List<int> asteroidPoolIdents;
    private Vector3 targetArea;
    private Vector3 targetPosition;
    private Vector3 spawnArea;
    private Vector3 spawnPosition;

    private int gameScore;
    private gameStates gameState;

    /// <summary>
    /// Animator's
    /// </summary>
    private Animator mainMenuAnimator;
    private Animator gameUIAnimator;

    /// <summary>
    /// Gui Elements
    /// </summary>
    public Text guiScore;

    /// <summary>
    /// Core game components
    /// </summary>

    private ShieldControl SC;
    private ObjectPool[] objectPools;
    private GameObject[] playerBuildings;

    /// <summary>
    /// Co routines
    /// </summary>

    private IEnumerator asteroidCoroutine;
    private IEnumerator menuMissileCoroutine;

    /// <summary>
    /// Preset important data and get object references.
    /// </summary>
	void Start () {

        // Store variables for later resets and preset variables.
        probabilityArray = new float[3];
        probabilityArray[0] = asteroid0Chance;
        probabilityArray[1] = asteroid1Chance;
        probabilityArray[2] = asteroid2Chance;

        hazardCountStore = hazardCount;
        asteroid0ChanceStore = asteroid0Chance;
        asteroid1ChanceStore = asteroid1Chance;
        asteroid2ChanceStore = asteroid2Chance;
        onTargetAsteroidsStore = onTargetAsteroids;

        
        targetPosition = targetZone.transform.position;
        spawnPosition = spawnZone.transform.position;
        targetArea = targetZone.GetComponent<Renderer>().bounds.size / 2;
        spawnArea = spawnZone.GetComponent<Renderer>().bounds.size / 2;

        gameScore = 0;
        gameState = gameStates.MenuMode;

        asteroidPoolIdents = new List<int>();
        asteroidPool = gameObject.GetComponent<ObjectPool>();
        asteroidPoolIdents.Add(asteroidPool.CreateObjectPool(20, asteroidTypes[0].gameObject));
        asteroidPoolIdents.Add(asteroidPool.CreateObjectPool(10, asteroidTypes[1].gameObject));
        asteroidPoolIdents.Add(asteroidPool.CreateObjectPool(5, asteroidTypes[2].gameObject));

        //Get animators
        mainMenuAnimator = GameObject.Find("Main_Menu").GetComponent<Animator>();
        gameUIAnimator = GameObject.Find("Game_UI").GetComponent<Animator>();

        //Get controller and other important game object references
        SC = GameObject.FindGameObjectWithTag("Shield").GetComponent<ShieldControl>();
        objectPools = FindObjectsOfType(typeof(ObjectPool)) as ObjectPool[];
        playerBuildings = GameObject.FindGameObjectsWithTag("Player");

        //Set coroutine identity's
        asteroidCoroutine = SpawnAsteroids();
        menuMissileCoroutine = MenuMissileAutoTarget();

        MenuBackGroundGame();
    }

    void Update()
    {
        if(gameState == gameStates.GameActive)
        {
            guiScore.text = "Score " + gameScore;
        }

    }

    void Reset()
    {
        probabilityArray = new float[3];
        probabilityArray[0] = asteroid0Chance;
        probabilityArray[1] = asteroid1Chance;
        probabilityArray[2] = asteroid2Chance;

        hazardCount = hazardCountStore;
        asteroid0Chance = asteroid0ChanceStore;
        asteroid1Chance = asteroid1ChanceStore;
        asteroid2Chance = asteroid2ChanceStore;
        onTargetAsteroids = onTargetAsteroidsStore;

        ResetBuildings();
        SC.Reset();
    }
    /// <summary>
    /// Start the Main menu background simulation
    /// </summary>
    public void MenuBackGroundGame() 
    {
        Reset();
        gameState = gameStates.MenuMode;
        SC.SetGameState(gameStates.MenuMode);
        ResetObjectPools();
        StartCoroutine(asteroidCoroutine);
        StartCoroutine(menuMissileCoroutine);
    }
    /// <summary>
    /// Start a new game!
    /// </summary>
    public void StartNewGame()
    {
        StopCoroutine(asteroidCoroutine);
        StopCoroutine(menuMissileCoroutine);
        ResetObjectPools();

        mainMenuAnimator.SetTrigger("NewGame");
        gameUIAnimator.SetTrigger("GameActive");

        gameState = gameStates.GameActive;
        SC.SetGameState(gameStates.GameActive);
        gameStartTime = Time.time;
        Invoke("InvokedCoroutineNewGame", 1);
    }
    /// <summary>
    /// Quit the game :(
    /// </summary>
    public void QuitGame()
    {
        Application.Quit();
    }
    /// <summary>
    /// Builds a probability array. The outcome does not need to add upto 100 (or 1);
    /// </summary>
    void BuildPossibilityArray()
    {
        asteroid2Chance *= asteroid2Multiplier;
        asteroid1Chance *= asteroid1Multiplier;

        probabilityArray[1] = asteroid1Chance;
        probabilityArray[2] = asteroid2Chance;
    }
    /// <summary>
    /// Asteroid spawn coroutine.
    /// </summary>
    /// <returns></returns>
    IEnumerator SpawnAsteroids()
    {
        yield return new WaitForSeconds(startWait);
        while (true)
        {
            //Perform difficulty changes every currentSpawnIncrementTime seconds
            if (Time.time > (gameStartTime + currentSpawnIncrementTime))
            {
                hazardCount += spawnIncrement;
                currentSpawnIncrementTime += spawnIncrementTime;
                BuildPossibilityArray();
                //increment the amount of targetted astyeroids;
                onTargetAsteroids += onTargetAsteroidsIncrement;

            }
            
            int x = 0;
            
            for (int i = 0; i < hazardCount; i++ )
            {
                

                int randomisedAsteroid = RandomiseAsteroid(probabilityArray);

                targetArea.y = targetZone.transform.position.y;
                spawnArea.y = spawnZone.transform.position.y;

                Vector3 targetPoint = new Vector3(Random.Range(targetPosition.x - targetArea.x, targetPosition.x + targetArea.x),
                                                    targetArea.y,
                                                    Random.Range(targetPosition.z - targetArea.z, targetPosition.z + targetArea.z));

                Vector3 spawnPoint = new Vector3(Random.Range(spawnPosition.x - spawnArea.x, spawnPosition.x + spawnArea.x),
                                                    spawnArea.y,
                                                    Random.Range(spawnPosition.z - spawnArea.z, spawnPosition.z + spawnArea.z));

                Quaternion spawnRotation = Quaternion.identity;

                GameObject pooledResetAsteroid = asteroidPool.GetObject(asteroidPoolIdents[randomisedAsteroid]);

                pooledResetAsteroid.GetComponent<AsteroidControl>().setTarget(targetPoint);
                pooledResetAsteroid.transform.position = spawnPoint;
                pooledResetAsteroid.transform.rotation = spawnRotation;
                pooledResetAsteroid.SetActive(true);

                if (x < onTargetAsteroids)
                {
                    SpawnOnTargetAsteroid();
                    x++;
                }
                yield return new WaitForSeconds(Random.Range(0, spawnWait));
            }


            yield return new WaitForSeconds(waveWait);

            if (gameState == gameStates.GameOver)
            {
                StopCoroutine(asteroidCoroutine);
                break;
            }
        }
    }
    /// <summary>
    /// Spawns a asteroid and targets it to a player building.
    /// </summary>
    void SpawnOnTargetAsteroid()
    {
        int randomisedAsteroid = RandomiseAsteroid(probabilityArray);

        spawnArea.y = spawnZone.transform.position.y;

        Vector3 targetPoint = GetActiveBuilding();

        Vector3 spawnPoint = new Vector3(Random.Range(spawnPosition.x - spawnArea.x, spawnPosition.x + spawnArea.x),
                                            spawnArea.y,
                                            Random.Range(spawnPosition.z - spawnArea.z, spawnPosition.z + spawnArea.z));

        Quaternion spawnRotation = Quaternion.identity;

        GameObject pooledResetAsteroid = asteroidPool.GetObject(asteroidPoolIdents[randomisedAsteroid]);

        pooledResetAsteroid.GetComponent<AsteroidControl>().setTarget(targetPoint);
        pooledResetAsteroid.transform.position = spawnPoint;
        pooledResetAsteroid.transform.rotation = spawnRotation;
        pooledResetAsteroid.SetActive(true);
    }
    /// <summary>
    /// Unity manuals nice clean probability chooser. Not mine.
    /// </summary>
    /// <param name="probs"></param>
    /// <returns></returns>
    int RandomiseAsteroid(float[] probs)
    {
        float total = 0;

        foreach (float elem in probs) {
            total += elem;
        }

        float randomPoint = Random.value * total;

        for (int i= 0; i < probs.Length; i++) {
            if (randomPoint < probs[i]) {
                return i;
            }
            else {
                randomPoint -= probs[i];
            }
        }
        return probs.Length - 1;
    }

    /// <summary>
    /// Spawns a specific type of asteroid.
    /// Used on asteroid split.
    /// </summary>
    /// <param name="type">int type of asteroid</param>
    /// <param name="position">Starting position</param>
    /// <param name="target">Target position</param>
    /// <param name="dispersion">Upper and lower limit for randomisation of target position (0 for none)</param>
    public void SpawnAsteroidType(int type, Vector3 position, Vector3 target, int dispersion)
    {
        Quaternion spawnRotation = Quaternion.identity;

        GameObject pooledResetAsteroid = asteroidPool.GetObject(asteroidPoolIdents[type]);

        Vector3 adjustedTarget = new Vector3(target.x + Random.Range(-dispersion, dispersion), target.y, target.z + Random.Range(-dispersion, dispersion));
        pooledResetAsteroid.GetComponent<AsteroidControl>().setTarget(adjustedTarget);
        pooledResetAsteroid.transform.position = position;
        pooledResetAsteroid.transform.rotation = spawnRotation;
        pooledResetAsteroid.SetActive(true);
    }
    /// <summary>
    /// Delegates a missile firing to a launcher on the respective side of the city
    /// </summary>
    /// <param name="target">The targets GameObject</param>
    public void DelegateToLauncher(GameObject target)
    {
        if (target.transform.position.x < 0)
        {
            if (missileLaunchers[0].GetComponent<LauncherControl>().GetAmmo() != 0)
            {
                missileLaunchers[0].GetComponent<LauncherControl>().FireWeapon(target);
            }
        }
        else
        {
            if (missileLaunchers[1].GetComponent<LauncherControl>().GetAmmo() != 0)
            {
                missileLaunchers[1].GetComponent<LauncherControl>().FireWeapon(target);
            }
        }
    }
    /// <summary>
    /// A simple auto targeting coroutine method for the menu screen background simulation.
    /// </summary>
    /// <returns></returns>
    public IEnumerator MenuMissileAutoTarget()
    {
        while (true)
        {
            Collider[] hitColliders = Physics.OverlapSphere(SC.transform.position, 40);

            if (hitColliders.Length > 0)
            {
                int i = 0;
                while (i < hitColliders.Length)
                {
                    if (hitColliders[i].gameObject.tag == "Asteroid")
                    {
                        DelegateToLauncher(hitColliders[i].gameObject);
                        break;
                    }
                    i++;
                }
             }
            yield return new WaitForSeconds(Random.Range(2,5));
        }

    }
    /// <summary>
    /// Resets all objects pools for a game start
    /// </summary>
    void ResetObjectPools()
    {
        foreach (ObjectPool op in objectPools)
        {
            op.Reset();
        }
    }
    /// <summary>
    /// Gets a active building for a asteroid to target.
    /// </summary>
    /// <returns></returns>
    Vector3 GetActiveBuilding()
    {
        //Identical almost to object pool get method
        for (int i = 0; i < playerBuildings.Length; i++)
        {
            if (playerBuildings[i].activeInHierarchy)
            {
                return playerBuildings[i].transform.position;
            }
        }
        //This game is over!
        gameState = gameStates.GameOver;
        gameUIAnimator.SetTrigger("GameOver");
        SC.SetGameState(gameStates.GameOver);
        //To ensure all code paths return something. Game Should be over at this point regardless.
        return new Vector3(Random.Range(spawnPosition.x - spawnArea.x, spawnPosition.x + spawnArea.x),
                           spawnArea.y,
                           Random.Range(spawnPosition.z - spawnArea.z, spawnPosition.z + spawnArea.z));
    }

    /// <summary>
    /// Resets all buildings.
    /// </summary>
    /// <returns></returns>
    void ResetBuildings()
    {
        for (int i = 0; i < playerBuildings.Length; i++)
        {
            if (!playerBuildings[i].activeInHierarchy)
            {
                playerBuildings[i].SetActive(true);
            }
        }
    }
    /// <summary>
    /// Needed to delay/invoke restarting the asteroid spawn coroutine.
    /// The coroutine needs a chance to stop before it can be restarted correctly.
    /// </summary>
    void InvokedCoroutineNewGame()
    {
        StartCoroutine(asteroidCoroutine);
    }

    public void CheckGameOverConditions()
    {
        //Reusing this method. Different purpose but the same code effectivly.
        GetActiveBuilding();
    }

    public void SetScore(int score)
    {
        gameScore += score;
    }
}
