using UnityEngine;
using UnityEngine.UI;
using System.Collections;

public class ShieldControl : MonoBehaviour {

    //Shield Settings
    public float shieldStrength;  //Max shield strength
    public float shieldReactivatePercentage;
    public float rechargeRate;
    public float rechargeMultiplyer;

    //Object variables
    private bool shieldDown;
    private float shieldReactivateAmount;
    private float currentShieldStrength;
    private bool playerTrigger;

    //UI references
    public Image shieldStrengthFor;
    public Image shieldReactivationDisplay;
    public Image shieldInnerButton;
    private Color shieldButtonOn;
    private Color shieldButtonOff;

    /// <summary>
    /// StateSettings
    /// </summary>
    private GameController.gameStates gameState;


    ///GameController
    private GameController GC;

    /// <summary>
    /// Calculation efficency saver;
    /// </summary>
    void Start()
    {
        GC = GameObject.FindGameObjectWithTag("GameController").GetComponent<GameController>();
        shieldReactivateAmount = (shieldStrength / 100) * shieldReactivatePercentage;
        currentShieldStrength = shieldStrength;

        shieldDown = false;

        shieldButtonOn = new Color (0, 200, 0, 255);
        shieldButtonOff = new Color (200, 0, 0, 255);

        gameState = GameController.gameStates.MenuMode;
    }

    public void Reset()
    {
        shieldReactivateAmount = (shieldStrength / 100) * shieldReactivatePercentage;
        currentShieldStrength = shieldStrength;

        shieldDown = false;
        GetComponent<MeshCollider>().enabled = true;
        GetComponent<MeshRenderer>().enabled = true;
    }

    public void RegisterShieldHit(int damageValue)
    {
        currentShieldStrength -= damageValue;
    }

    void Update()
    {
        switch (gameState)
        {
            case GameController.gameStates.MenuMode:
                {
                    return;
                }
            case GameController.gameStates.GameActive:
                {
                    shieldStrengthFor.fillAmount = (1.0f / shieldStrength) * currentShieldStrength;

                    if (shieldDown)
                    {
                        currentShieldStrength += (rechargeRate * Time.deltaTime) * rechargeMultiplyer;
                    }
                    else if (currentShieldStrength < shieldStrength)
                    {
                        currentShieldStrength += rechargeRate * Time.deltaTime;
                    }

                    if (currentShieldStrength <= 0 && !shieldDown)
                    {
                        shieldDown = true;
                        GetComponent<MeshCollider>().enabled = false;
                        GetComponent<MeshRenderer>().enabled = false;
                        shieldInnerButton.color = shieldButtonOff;
                    }

                    if (currentShieldStrength >= shieldStrength && shieldDown)
                    {
                        shieldDown = false;
                        GetComponent<MeshCollider>().enabled = true;
                        GetComponent<MeshRenderer>().enabled = true;
                        shieldInnerButton.color = shieldButtonOn;
                    }

                    return;
                }
            case GameController.gameStates.GameOver:
                {                   
                    return;
                }
        }

    }

    public void SetGameState(GameController.gameStates newState)
    {
        gameState = newState;
    }

    public void TriggerShield()
    {
        if (shieldDown == false)
        {
            shieldDown = true;
            GetComponent<MeshCollider>().enabled = false;
            GetComponent<MeshRenderer>().enabled = false;
            shieldInnerButton.color = shieldButtonOff;
        }
        else if (shieldDown == true && currentShieldStrength >= shieldReactivateAmount)
        {
            shieldDown = false;
            GetComponent<MeshCollider>().enabled = true;
            GetComponent<MeshRenderer>().enabled = true;
            shieldInnerButton.color = shieldButtonOn;
        }
    }
}
