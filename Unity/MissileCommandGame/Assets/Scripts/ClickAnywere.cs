using UnityEngine;
using System.Collections;

public class ClickAnywere : MonoBehaviour {

	// Update is called once per frame
	void Update () {
        if (Input.GetMouseButtonDown(0))
        {
            GameObject.Find("Game_UI").GetComponent<Animator>().SetTrigger("MainMenu");
            GameObject.Find("Main_Menu").GetComponent<Animator>().SetTrigger("GameOver");
            GameObject.Find("GameController").GetComponent<GameController>().MenuBackGroundGame();
        }
            
	}
}
