using UnityEngine;
using System.Collections;

public class GunControl : MonoBehaviour {

    public GameObject xAxisBone;
    public GameObject yAxisBone;
    public GameObject gunBody;
    public GameObject muzzle;
    public int maxRange;
    public float rotationSpeed;
    public GameObject weaponType;
    public float projectileVelocity;
    public float rateOfFire;
    
    private GameObject currentTarget;
    private bool hasTarget;
    private ObjectPool projectiles;
    private int ObjectPoolIdent;
    private float timeElapsed;

	void Start () {

        currentTarget = null;
        hasTarget = false;
        projectiles = gameObject.GetComponent<ObjectPool>();
        ObjectPoolIdent = projectiles.CreateObjectPool(20, weaponType);

        rateOfFire = 1 / rateOfFire;
	}
	
	void Update () {

        Debug.DrawRay(muzzle.transform.position, -muzzle.transform.up * 20.0f);
        
        timeElapsed += Time.deltaTime;
        
        if(!hasTarget)
        {
            hasTarget = TargetAsteroid(transform.position, maxRange);
        }
	
        if (hasTarget)
        {
            AimTurret();
            FireWeapon();
        }
	}

    /// <summary>
    /// Locates a target for the gun within a defined sphere using physics colliders.
    /// Tests sqr magnitude for the closest one if it exists.
    /// </summary>
    /// <param name="center"></param>
    /// <param name="radius"></param>
    /// <returns>True if target found false if not</returns>
    bool TargetAsteroid(Vector3 center, float radius)
    {
        Collider[] hitColliders = Physics.OverlapSphere(center, radius);

        if (hitColliders.Length > 0)
        {
            float ClosestSqrDistance = 99999999;
            Collider ClosestCollider = null;
            int i = 0;
            while (i < hitColliders.Length)
            {
                if (hitColliders[i].gameObject.tag == "Asteroid")
                {
                
                    Vector3 Offset = hitColliders[i].transform.position - transform.position;

                    if (i == 0)
                    {
                        ClosestSqrDistance = Offset.sqrMagnitude;
                        ClosestCollider = hitColliders[i];
                        i++;
                        continue;
                    }

                    if (Offset.sqrMagnitude < ClosestSqrDistance)
                    {
                        ClosestSqrDistance = Offset.sqrMagnitude;
                        ClosestCollider = hitColliders[i];
                        i++;
                    }
                    i++;
                }
                else
                {
                    i++;
                }
            }
            if(ClosestCollider != null)
            {
                currentTarget = ClosestCollider.gameObject;
                currentTarget.GetComponent<AsteroidControl>().targetedByGun(this);
                return true;
            }
        }

        return false;

    }

    public void TargetDestroyed()
    {
        hasTarget = false;
        currentTarget = null;
    }
    /// <summary>
    /// Aims the turret model at the current target. Rotates two joints to align the gunBody at the target.
    /// </summary>
    void AimTurret()
    {
        //Get the vector to the target
        Vector3 aimAt = CalculateLead(currentTarget) - gunBody.transform.position;
        Quaternion lookAtRotation = Quaternion.LookRotation(aimAt, gunBody.transform.forward);
        //Temp store to split axis of rotation
        Quaternion temp = Quaternion.Slerp(gunBody.transform.rotation, lookAtRotation, Time.time * rotationSpeed);
        //Rotate only the y axis in world space but the models axis are all messed up so its z for local space.
        yAxisBone.transform.localRotation = Quaternion.Euler(new Vector3(0f, 0f, temp.eulerAngles.y));
        //Rotate only the x axis for this joint
        xAxisBone.transform.localRotation = Quaternion.Euler(new Vector3(temp.eulerAngles.x + 90.0f, 0f , 0f));

    }

    void FireWeapon()
    {
        if(timeElapsed >= rateOfFire)
        {
            GameObject shot = projectiles.GetObject(ObjectPoolIdent);
            shot.transform.position = muzzle.transform.position;
            shot.transform.rotation = muzzle.transform.rotation;
            shot.SetActive(true);
            shot.GetComponent<Rigidbody>().velocity = -muzzle.transform.up * projectileVelocity;
            timeElapsed = 0.0f;
        }
    }
    /// <summary>
    /// I wish I had to the time to properly digest the proof for this and work something of my own out, 
    /// but I'm simply short on time. Reinventing the wheel isn't my style also without a good reason. 
    /// Needless to say I will be studying it closely when I have more time as it is the most elegant 
    /// solution I've seen to date.
    /// http://www.tosos.com/pages/calculating-a-lead-on-a-target/
    /// 
    /// I explored hard coding a lead object ahead of each asteroid but thats just not good enough...
    /// </summary>
    /// <param name="target"></param>
    /// <returns>The adjusted lead Vector3</returns>
    Vector3 CalculateLead(GameObject target)
    {
        Vector3 V = target.GetComponent<Rigidbody>().velocity;
        Vector3 D = target.transform.position - transform.position;
        float A = V.sqrMagnitude - projectileVelocity * projectileVelocity;
        float B = 2 * Vector3.Dot(D, V);
        float C = D.sqrMagnitude;
        if (A >= 0)
        {
            Debug.LogError("No solution exists");
            return target.transform.position;
        }
        else
        {
            float rt = Mathf.Sqrt(B * B - 4 * A * C);
            float dt1 = (-B + rt) / (2 * A);
            float dt2 = (-B - rt) / (2 * A);
            float dt = (dt1 < 0 ? dt2 : dt1);
            return target.transform.position + V * dt;
        }
    }
}
