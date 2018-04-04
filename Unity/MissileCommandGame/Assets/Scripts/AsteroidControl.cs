using UnityEngine;
using System.Collections;
using System.Collections.Generic;

public class AsteroidControl : MonoBehaviour {

    public int[] splitTypes;
    
    private Vector3 targetPoint;
    private Rigidbody Rb;
    private List<GunControl> targetedBy;
    private GameController GC;

    void Awake()
    {
        Rb = GetComponent<Rigidbody>();
        targetedBy = new List<GunControl>();
        GC = GameObject.FindGameObjectWithTag("GameController").GetComponent<GameController>();
    }

    void OnEnable ()
    {
        Vector3 targetVector = targetPoint - transform.position;
        targetVector.Normalize();

        Rb.velocity = targetVector;
	}

    void OnMouseDown()
    {
        GC.DelegateToLauncher(gameObject);
    }

    public void setTarget(Vector3 v)
    {
        targetPoint = v;
    }

    public Vector3 getTarget()
    {
        return targetPoint;
    }

    public void targetedByGun(GunControl gun)
    {
        targetedBy.Add(gun);
    }

    public void SplitAsteroid()
    {
        if (splitTypes.Length > 0)
        {
            for (int i = 0; i < splitTypes.Length; i++)
            {
                if (splitTypes[i] == 0)
                {
                    GC.SpawnAsteroidType(0, gameObject.transform.position, targetPoint, 5);
                }
                else if (splitTypes[i] == 1)
                {
                    GC.SpawnAsteroidType(1, gameObject.transform.position, targetPoint, 5);
                }
                else
                {
                    Debug.Log("Unknown asteroid type to split");
                }
            }
        }
    }
    /// <summary>
    /// Method for implementing a asteroid impact with a game object.
    /// Includes object pool reset and particle effect calls.
    /// </summary>
    public void Impact()
    {
        DisableAsteroid();
    }

    public void DisableAsteroid()
    {  
        
        gameObject.SetActive(false); 
     
    }

    void OnDisable()
    {
        transform.position = Vector3.zero;
        transform.rotation = Quaternion.Euler(Vector3.zero);
        Rigidbody rb = GetComponent<Rigidbody>();
        rb.angularVelocity = Vector3.zero;
        rb.velocity = Vector3.zero;

        foreach (GunControl gun in targetedBy)
        {
            gun.TargetDestroyed();
        }
    }
}
