using UnityEngine;
using System.Collections;

public class CollisionDetectAsteroid : MonoBehaviour {

    public GameObject explosion;
    public int scoreValue;
    public int hitPointValue;

    private int currentDmg;
    private AsteroidControl asteroidController;
    private GameController gameController;

    void Start()
    {
        GameObject gameControllerObject = GameObject.FindWithTag("GameController");
        asteroidController = GetComponent<AsteroidControl>();
        if (gameControllerObject != null)
        {
            gameController = gameControllerObject.GetComponent<GameController>();
        }
        if (gameController == null)
        {
            Debug.Log("Cannot find 'GameController' script");
        }
    }

    void OnEnable()
    {
        currentDmg = hitPointValue;
    }
    /// <summary>
    /// Does the asteroid need to split?
    /// </summary>
    void Update()
    {
        if (currentDmg <= 0)
        {
            gameController.SetScore(scoreValue);
            asteroidController.SplitAsteroid();
            asteroidController.DisableAsteroid();           
        }
    }

    void OnTriggerEnter(Collider other)
    {
        if (other.tag == "Asteroid")
        {
            return;
        }
        //Collisions with the player buildings. Calls for a game over check
        if (other.tag == "Player")
        {
            other.gameObject.SetActive(false);
            GetComponent<AsteroidControl>().Impact();
            gameController.CheckGameOverConditions();
        }
        //Collisions with the shield. Applies its damage value to the shield
        if (other.tag == "Shield")
        {
            other.GetComponent<ShieldControl>().RegisterShieldHit(scoreValue);
            GetComponent<AsteroidControl>().Impact();
        }
        //Collisions with a missile. Takes the damage value of the missile and calls it to detonate.
        if (other.tag == "Missile")
        {
            MissileControl mc = other.GetComponent<MissileControl>();
            currentDmg -= mc.missileDamage;
            mc.DetonateMissile();
        }
        //Collision with a projectile. Takes the damage value of the projectile and calls for it
        //to be destroyed
        if (other.tag == "Projectile")
        {
            ProjectileControl pc = other.GetComponent<ProjectileControl>();
            currentDmg -= pc.damageValue;
            pc.Destroy();
        }
        //Collision with the game surface. Nothing to see here.
        if (other.tag == "TargetSurface")
        {
            gameObject.GetComponent<AsteroidControl>().DisableAsteroid();
        }


    }
}
