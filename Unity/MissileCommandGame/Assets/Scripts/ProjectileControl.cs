using UnityEngine;
using System.Collections;

public class ProjectileControl : MonoBehaviour {

    public int damageValue;
    
    /// <summary>
    /// A standard unity concept to kill limited life span objects.
    /// </summary>
	void OnEnable()
    {
        Invoke("Destroy", 3f);
    }
    /// <summary>
    /// Destroys the projectile and returns it to the object pool. Implements Destroy required by
    /// the object pool.
    /// </summary>
    public void Destroy()
    {
        gameObject.SetActive(false);
    }
    /// <summary>
    /// 
    /// </summary>
    void OnDisable()
    {
        transform.position = Vector3.zero;
        transform.rotation = Quaternion.Euler(Vector3.zero);
        Rigidbody rb = GetComponent<Rigidbody>();
        rb.angularVelocity = Vector3.zero;
        rb.velocity = Vector3.zero;

        CancelInvoke();
    }
}
