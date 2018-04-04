using UnityEngine;
using System.Collections;

public class CollisionDetectMissile : MonoBehaviour {

    void OnTriggerEnter(Collider other)
    {
        if (other.tag == "TargetSurface")
        {
            GetComponent<MissileControl>().DetonateMissile(); ;
        }
    }
}
