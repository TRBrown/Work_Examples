using UnityEngine;
using System.Collections;

public class LauncherControl : MonoBehaviour
{

    public GameObject xAxisBone;
    public GameObject yAxisBone;
    public GameObject launcherBody;
    public GameObject missileLauncher;
    public Animator MissileUI;

    public float rotationSpeed;
    public GameObject weaponType;
    public float missileVelocity;
    public int weaponAmmo;
    public float weaponReload;
    private ObjectPool missiles;
    private int ObjectPoolIdent;
    private float timeElapsed;
    private int currentAmmo;

    void Start()
    {
        missiles = gameObject.GetComponent<ObjectPool>();
        ObjectPoolIdent = missiles.CreateObjectPool(6, weaponType);
        timeElapsed = weaponReload;
        currentAmmo = 0;
    }

    void Update()
    {
        Debug.DrawRay(missileLauncher.transform.position, -missileLauncher.transform.up * 20.0f);

    }

    /// <summary>
    /// Aims the turret model at the current target. Rotates two joints to align the gunBody at the target.
    /// </summary>
    void AimTurret(GameObject target)
    {
        //Get the vector to the target
        Vector3 aimAt = target.transform.position - launcherBody.transform.position;
        Quaternion lookAtRotation = Quaternion.LookRotation(aimAt, launcherBody.transform.forward);
        //Temp store to split axis of rotation
        Quaternion temp = Quaternion.Slerp(launcherBody.transform.rotation, lookAtRotation, Time.time * rotationSpeed);
        //Rotate only the y axis in world space but the models axis are all messed up so its z for local space.
        yAxisBone.transform.localRotation = Quaternion.Euler(new Vector3(0f, 0f, temp.eulerAngles.y));
        //Rotate only the x axis for this joint
        xAxisBone.transform.localRotation = Quaternion.Euler(new Vector3(temp.eulerAngles.x + 90.0f, 0f, 0f));

    }

    public bool FireWeapon(GameObject target)
    {
        AimTurret(target);

        if (currentAmmo > 0)
        {
            GameObject shot = missiles.GetObject(ObjectPoolIdent);
            shot.SetActive(true);
            shot.transform.position = missileLauncher.transform.position;
            shot.transform.rotation = Quaternion.LookRotation(-missileLauncher.transform.up);
            shot.GetComponent<MissileControl>().target = target;
            shot.GetComponent<MissileControl>().SetHoming();
            shot.GetComponent<Rigidbody>().velocity = -missileLauncher.transform.up * missileVelocity;
            currentAmmo--;
            MissileUI.SetTrigger("LaunchMissile");
            return true;
        }
        return false;
    }

    public int GetAmmo()
    {
        return currentAmmo;
    }

    public void Reload()
    {
        currentAmmo = weaponAmmo;
    }

}

