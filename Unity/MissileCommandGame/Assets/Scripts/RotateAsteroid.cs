using UnityEngine;
using System.Collections;

public class RotateAsteroid : MonoBehaviour {

    public float roll;
    private Rigidbody Rb;

    void Awake()
    {
        Rb = GetComponent<Rigidbody>();
    }

    void OnEnable()
    {
        Rb.angularVelocity = Random.insideUnitSphere * roll;
    }
}
