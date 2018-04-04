using UnityEngine;
using System.Collections;

public class RotateAround : MonoBehaviour {

    public Vector3 rotationIncrement;
    private Vector3 rotationAngle;
	
	// Update is called once per frame
	void FixedUpdate () {

        rotationAngle.x += rotationIncrement.x;
        rotationAngle.y += rotationIncrement.y;
        rotationAngle.z += rotationIncrement.z;

        transform.rotation = Quaternion.Euler(rotationAngle);

        if (rotationAngle.x > 360.0f)
        {
            rotationAngle.x -= 360.0f;
        }
        if (rotationAngle.y > 360.0f)
        {
            rotationAngle.y -= 360.0f;
        }
        if (rotationAngle.y > 360.0f)
        {
            rotationAngle.y -= 360.0f;
        }
	}
}
