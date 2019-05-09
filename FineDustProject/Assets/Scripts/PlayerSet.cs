﻿using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

using Game.Protocol;

public class PlayerSet : MonoBehaviour
{
    private InputField ip;
    public GameObject playerObj;
    public static GameObject players;
    public static PlayerStatus playerStatus;

    public int num = 1;

    private void Awake()
    {
        CreateAllPlayer();
    }

    void CreateAllPlayer()
    {
        //players = new GameObject[Game.Protocol.Protocol.MAX_USER];
        //playerStatus = new PlayerStatus[Game.Protocol.Protocol.MAX_USER];

        //for (int i = 0; i<Game.Protocol.Protocol.MAX_USER; ++i)
        //{
        //    Vector3 spawnPos = Vector3.zero;
        //    if (i == 1)
        //    {
        //        spawnPos = new Vector3(-1, 0, -6);
        //    }
        //    else if (i == 2)
        //    {
        //        spawnPos = new Vector3(1, 0, -6);
        //    }
        //    else if (i == 3)
        //    {
        //        spawnPos = new Vector3(-1, 0, -7);
        //    }
        //    else if (i == 4)
        //    {
        //        spawnPos = new Vector3(1, 0, -7);
        //    }
        //players[i] = GameObject.Instantiate(playerObj, spawnPos, Quaternion.identity) as GameObject;
        //players[i].SetActive(false);
        //playerStatus[i] = FindObjectOfType<PlayerStatus>();

        //}
    }

    public void Spawn()
    {
        //for(int i = 0; i< Game.Network.NetWork.client_data.Count;++i)
        //{
        //    if (i != 0 && i != Game.Network.NetWork.Client_id)
        //    {
        //        int other_id = Game.Network.NetWork.client_data[i].get_id();
        //        players[other_id].SetActive(true);
        //    }
        //}
        //for (int i = 0; i < Game.Protocol.Protocol.MAX_USER; ++i)
        //{
        //if (i != 0 && i != Game.Network.NetWork.Client_id)
        //{
        //          if(playerStatus[i].connect==true)
        //               players[i].SetActive(true);
        //}
        //}
        //playerStatus = FindObjectOfType<PlayerStatus>();

        //playerStatus.ID = num;
        //}
        for(int i =1; i<= Game.Network.NetWork.Client_id; ++i)
        {
            string a = "Player(" + i.ToString() + ")";

            playerObj = GameObject.Find("Players").transform.Find(a).gameObject;
            playerStatus = playerObj.GetComponent<PlayerStatus>();

            if (i == playerStatus.ID)
                playerObj.SetActive(true);
        }
    }
}