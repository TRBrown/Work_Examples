using UnityEngine;
using System.Collections;

public class RightLauncherAnimationEvent : MonoBehaviour
{

    // Use this for initialization
    public void ReloadAnimationEnd()
    {
        GameObject.Find("MissilePlatform_2").GetComponent<LauncherControl>().Reload(); ;
    }
}
