using UnityEngine;
using System.Collections;

public class LeftLauncherAnimationEvent : MonoBehaviour {

	// Use this for initialization
	public void ReloadAnimationEnd()
    {
        GameObject.Find("MissilePlatform_1").GetComponent<LauncherControl>().Reload(); ;
    }
}
