using UnityEngine;
using System.Collections;

public class MissileControl : MonoBehaviour
{

    public GameObject target;
    public float activeDelay;
    public int thrust;
    public int turnSpeed;
    public float dumbFireTimer;
    public int missileDamage;

    private bool dumbFire;
    private bool active;
    private Rigidbody Rb;
    private float timeFired;
    private float timeTargetLost;
    private Vector3 movemnetDir;
    private bool waitForParticles;
    private ParticleSystem smokeTrail;

    //Since the object is pooled pre setup must be done before start as
    //start exist behind OnEnable in the call hierarchy.
    void Awake()
    {
        Rb = GetComponent<Rigidbody>();
    }
    /// <summary>
    /// Resets the missile components.
    /// N.B. Messy dont like this but have little choice with out reoganising the missile
    /// components.
    /// </summary>
    void OnEnable()
    {
        timeFired = Time.time;
        dumbFire = true;
        gameObject.transform.FindChild("Explosion").gameObject.SetActive(false);
        Rb.isKinematic = false;
        gameObject.GetComponent<CapsuleCollider>().enabled = true;
        gameObject.transform.FindChild("Missile_Body").gameObject.SetActive(true);
        gameObject.transform.FindChild("Missile_side_Ext").gameObject.SetActive(true);
        gameObject.transform.FindChild("Engines").gameObject.SetActive(false);
        GameObject enginesParent = gameObject.transform.FindChild("Engines").gameObject;
        enginesParent.transform.FindChild("EngineLight").gameObject.SetActive(true);

        ParticleSystem[] childParticleSytems = enginesParent.GetComponentsInChildren<ParticleSystem>();
        foreach (ParticleSystem childPS in childParticleSytems)
        {
            if (childPS.name == "Smoke_trail")
            {
                smokeTrail = childPS;
            }
            childPS.Play();
        }
    }
    /// <summary>
    /// Update method checks for ca lost target to set the missile as dumbfire.
    /// Also checks for the particle effects on detonation to finish to reset the
    /// missile.
    /// </summary>
    void Update()
    {
        if (target.activeSelf == false && !dumbFire)
        {
            dumbFire = true;
            timeTargetLost = Time.time;
        }
        
        if (waitForParticles)
        {
            if (smokeTrail.isStopped)
            {
                ResetMissile();
            }
        }
    }
    /// <summary>
    /// Here we perform the physics rigid body updates for the missile pre and post ignition.
    /// </summary>
    void FixedUpdate()
    {
        //If the missile is homing and active apply thrust to the normalised vector to the target.
        //Also incrementally rotate the model by turnSpeed towards the target.
        if (!dumbFire && active)
        {
            movemnetDir = target.transform.position - gameObject.transform.position;
            Rb.AddForce(movemnetDir.normalized * thrust);
            Quaternion targetRotation = Quaternion.LookRotation(Rb.velocity);
            float step = turnSpeed * Time.deltaTime;
            transform.rotation = Quaternion.RotateTowards(transform.rotation, targetRotation, step);

            
            Debug.DrawRay(gameObject.transform.position, movemnetDir * 20.0f, Color.cyan);

            return;
        }

        if (dumbFire && active)
        {
            //Still flying
            Rb.AddForce(movemnetDir.normalized * thrust);
            // Dont want it flying at a angle 
            Quaternion targetRotation = Quaternion.LookRotation(Rb.velocity);
            transform.rotation = Quaternion.RotateTowards(transform.rotation, targetRotation, turnSpeed);
            
            if (Time.time >= timeTargetLost + dumbFireTimer)
            {
                DetonateMissile();
            }
            return;
        }
        // 0.5 seconds after timeFired the missile is set to active and particle effect start
        if (Time.time > (timeFired + activeDelay) && !waitForParticles)
        {
            active = true;
            gameObject.transform.FindChild("Engines").gameObject.SetActive(true);
        }

    }
    //Setter for a homing and dumb fire mode
    public void SetHoming()
    {
        if (target != null)
        {
            dumbFire = false;
        }
        else
        {
            Debug.Log("Missile set to homing with no target");
        }
    }

    /// <summary>
    /// Activates and deactivates missile components to account allow for particle effects
    /// to finish.
    /// N.B. This is messy I could of made it more efficient by reorganising the object
    /// components but didn't have time.
    /// </summary>
    public void DetonateMissile()
    {
        gameObject.transform.FindChild("Explosion").gameObject.SetActive(true);
        Rb.isKinematic = true;
        gameObject.GetComponent<CapsuleCollider>().enabled = false;
        gameObject.transform.FindChild("Missile_Body").gameObject.SetActive(false);
        gameObject.transform.FindChild("Missile_side_Ext").gameObject.SetActive(false);
        GameObject enginesParent = gameObject.transform.FindChild("Engines").gameObject;
        enginesParent.transform.FindChild("EngineLight").gameObject.SetActive(false);
        
        ParticleSystem[] childParticleSytems = enginesParent.GetComponentsInChildren<ParticleSystem>();
        foreach (ParticleSystem childPS in childParticleSytems)
        {
            if (childPS.name == "Smoke_trail")
            {
                smokeTrail = childPS;
            }
            childPS.Stop();
        }
        waitForParticles = true;
        active = false;
    }

    /// <summary>
    /// Reset missile variables and gameObject to false;
    /// </summary>
    void ResetMissile()
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
        dumbFire = true;
        timeFired = 0;
        target = null;
        waitForParticles = false;
    }

    /// <summary>
    /// Function for AOE damage by missiles;
    /// </summary>
    /// <param name="centerPosition"></param>
    /// <param name="explosionRadius"></param>
    void AOEDamage(Vector3 centerPosition, float explosionRadius)
    {
        //Get active collider's hit my the explosion
        Collider[] hitColliders = Physics.OverlapSphere(centerPosition, explosionRadius);
        int i = 0;
        while (i < hitColliders.Length)
        {
            //Do damage.
        }
    }
}





