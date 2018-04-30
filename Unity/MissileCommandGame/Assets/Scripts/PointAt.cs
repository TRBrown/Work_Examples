using UnityEngine;
using System.Collections;

public class PointAt : MonoBehaviour {

    public GameObject FocusPoint;
	void Start () {
	
	}
	
	// Update is called once per frame
	void FixedUpdate () {

        Vector3 movemnetDir = FocusPoint.transform.position - transform.position;

        transform.rotation = Quaternion.FromToRotation(Vector3.forward, movemnetDir);
	
	}
}
