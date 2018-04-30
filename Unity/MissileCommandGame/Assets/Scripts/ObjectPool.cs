using UnityEngine;
using System.Collections;
using System.Collections.Generic;

public class ObjectPool : MonoBehaviour {

    private List<List<GameObject>> objectPool;
    private List<GameObject> pooledObject;
    private int numofPools;
    public bool autoExpand;

    void Awake()
    {
        numofPools = 0;
        objectPool = new List<List<GameObject>>();
        pooledObject = new List<GameObject>();
    }

    public int CreateObjectPool(int poolSize, GameObject preFabType)
    {     
        pooledObject.Add(preFabType);
        objectPool.Add(new List<GameObject>());

        for(int i = 0;i < poolSize; i++)
        {
            GameObject newObj = Instantiate(preFabType);
            newObj.SetActive(false);
            objectPool[numofPools].Add(newObj);
        }
        numofPools++;
        return numofPools - 1;
    }

    public GameObject GetObject(int poolIdent)
    {
        for(int i = 0; i < objectPool[poolIdent].Count; i++)
        {
            if (!objectPool[poolIdent][i].activeInHierarchy)
            {
                return objectPool[poolIdent][i];
            }
        }

        if (autoExpand)
        {
            GameObject newObj = Instantiate(pooledObject[poolIdent]);
            newObj.SetActive(false);
            objectPool[poolIdent].Add(newObj);
            return newObj;
        }
        return null;
    }

    public void Reset()
    {
        for (int i = 0; i < objectPool.Count; i++)
        {
            for (int x = 0; x < objectPool[i].Count; x++)
            {
                if (objectPool[i][x].activeInHierarchy)
                {
                    objectPool[i][x].SetActive(false);
                }
            }
        }
    }

}
