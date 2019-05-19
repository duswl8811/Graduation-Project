﻿using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.AI;

public class Item : MonoBehaviour
{
    public bool isCollision = false;
    public GameObject player;
    Rigidbody rigidbody;

    public enum TYPE { Box, Crystal }

    public TYPE type;           // 아이템의 타입.
    public Sprite DefaultImg;   // 기본 이미지.
    public int MaxCount;        // 겹칠수 있는 최대 숫자.

    private Inventory Iv;

    void Awake()
    {
        // 태그명이 "Inventory"인 객체의 GameObject를 반환한다.
        // 반환된 객체가 가지고 있는 스크립트를 GetComponent를 통해 가져온다.
        Iv = GameObject.FindGameObjectWithTag("Inventory").GetComponent<Inventory>();
        rigidbody = GetComponent<Rigidbody>();
        Vector3 spawnPosition = (transform.position);
        NavMeshHit hit;
        if (NavMesh.SamplePosition(spawnPosition, out hit, 50.0f, NavMesh.AllAreas))
        {
            Debug.Log("hit"+hit.position);
            Debug.Log("spawn"+spawnPosition);
            spawnPosition.y = hit.position.y;
            transform.position = new Vector3(spawnPosition.x, spawnPosition.y, spawnPosition.z);
        }
    }

    void Start()
    {
        
    }

    void Update()
    {

        if (isCollision)
        {
            if (Input.GetKey(KeyCode.F))
            {
                Destroy(gameObject);
                AddItem();
                Debug.Log("냠");
            }
        }
        
    }

    void AddItem()
    {
        // 아이템 획득에 실패할 경우.
        if (!Iv.AddItem(this))
            Debug.Log("아이템이 가득 찼습니다.");
        else // 아이템 획득에 성공할 경우.
            gameObject.SetActive(false); // 아이템을 비활성화 시켜준다.
    }

    void OnCollisionEnter(Collision collision)
    {
        if (collision.gameObject.CompareTag("Player"))
            isCollision = true;

        if (collision.gameObject.CompareTag("Terrain"))
        {
            if (null != rigidbody)

                Destroy(GetComponent<Rigidbody>());
            Debug.Log("땅에 만난 템");
        }

    }

    void OnCollisionExit(Collision collision)
    {
        if (collision.gameObject.CompareTag("Player"))
            isCollision = false;
    }
}
