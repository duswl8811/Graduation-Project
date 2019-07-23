﻿using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.AI;

public class MonsterMgr : MonoBehaviour
{
    public Transform player_tf;
    public PlayerStatus player_st;
    NavMeshAgent nav;
    //new Rigidbody rigidbody;
    MonsterSpawner Monster_Spawner;

    public enum ANI_TYPE { IDEL, WALK, RUN, JUMP };

    public int HP;
    public bool isCollision = false;
    public bool is_KnockBack = false;
    public bool is_Wait = false;
    public bool is_Tracking = false;    // tracking이 on일 때 player의 미세먼지 흡입량을 증가시키기 위해서
    public bool is_BackToSpawn = false;
    
    public float targetRange = 20.0f;
    public float KnockBack_Time = 0.1f;
    public float Waiting_Time = 0.0f;

    public Vector3 Random_Position;
    public float default_velocity;

    public Vector3 spawnPosition;

    public int ID;

    void Awake()
    {
        nav = GetComponent<NavMeshAgent>();

        Monster_Spawner = GameObject.Find("MonsterSpawner").GetComponent<MonsterSpawner>();

        //rigidbody = GetComponent<Rigidbody>();
        spawnPosition = (transform.position);
        NavMeshHit hit;
        if (NavMesh.SamplePosition(spawnPosition, out hit, 50.0f, NavMesh.AllAreas))
        {
            spawnPosition.y = hit.position.y;
            transform.position = new Vector3(spawnPosition.x, spawnPosition.y, spawnPosition.z);
            nav.Warp(new Vector3(spawnPosition.x, spawnPosition.y, spawnPosition.z));
        }

        ID = Monster_Spawner.monID;
    }

    void Start()
    {
        player_tf = GameObject.FindGameObjectWithTag("Player").transform;   // 플레이어의 정보값
        Debug.Log(player_tf.gameObject.name);
        player_st = player_tf.GetComponent<PlayerStatus>();
        
        targetRange = 20.0f;
        HP = 100;

        Random_Position = transform.position;
    }

    // Update is called once per frame
    void Update()
    {

        if (transform.position.y < 20)
        {
            Destroy(gameObject);
            Monster_Spawner.monCnt--;
        }

        if (Vector3.Distance(Random_Position, spawnPosition) > 80)
        {
            Random_Position += (spawnPosition - Vector3.zero);
        }
        else
        {
            Random_Position.x += Random.Range(-0.5f, 0.5f);
            Random_Position.z += Random.Range(-0.5f, 0.5f);
        }

        if (player_st.Ani_State_Walk_Run == PlayerStatus.ANI_TYPE.RUN)
            nav.speed = 20;
        else if (player_st.Ani_State_Walk_Run == PlayerStatus.ANI_TYPE.WALK)
            nav.speed = 10;
        else nav.speed = 3;
        
        var _distance = Vector3.Distance(player_tf.transform.position, transform.position);   // 몬스터와 플레이어 사이 거리
        var spawn_distance = Vector3.Distance(spawnPosition, transform.position);   // 스폰 영역 밖으로 나갔는지 확인 할 거리

        if (spawn_distance < 100)   // 스폰 영역 내일때
        {
            // 몬스터 플레이어 따라가기
            if ((2f <= _distance) && (_distance <= targetRange) && (!is_Wait))
            {
                is_Tracking = true;
            }

            else
            {
                is_Tracking = false;
            }
        }
        else    // 스폰 영역 밖일때
        {
            is_BackToSpawn = true;
            is_Tracking = false;
        }

        if (is_Tracking)
        {
            nav.SetDestination(player_tf.transform.position);
        }

        if (!is_Tracking)
        {
            nav.SetDestination(Random_Position);
        }

        if (is_BackToSpawn)
        {
            nav.SetDestination(spawnPosition);

            if (transform.position == spawnPosition)
                is_BackToSpawn = false;
        }

        // 플레이어가 몬스터 공격
        if (isCollision)
        {
            if (Input.GetMouseButtonUp(0)) //0:좌 1:중 2:우
            {
                HP -= 10;
                Debug.Log("피 " + HP + "남음");
                is_KnockBack = true;
                KnockBack_Time = 0.1f;
            }
        }

        if (is_KnockBack)
        {
            transform.Translate((Vector3.back) * KnockBack_Time);
            KnockBack_Time += Time.deltaTime;
            if (KnockBack_Time > 0.3)
            {
                is_KnockBack = false;
                is_Wait = true;
                Waiting_Time = 0.0f;
            }
        }

        if (is_Wait)
        {
            Waiting_Time += Time.deltaTime;
            if (Waiting_Time > 0.5) is_Wait = false;
        }


        if (HP == 0)
        {
            Destroy(gameObject);
            Debug.Log("멧돼지 사망");
            Monster_Spawner.monCnt--;
        }
        
    }

    void OnCollisionEnter(Collision collision)
    {
        if (collision.gameObject.CompareTag("Player"))
        {
            isCollision = true;
        }
    }

    void OnCollisionExit(Collision collision)
    {
        if (collision.gameObject.CompareTag("Player"))
            isCollision = false;
    }

    void OnTriggerStay(Collider other)
    {
            Debug.Log(other.gameObject.name);
            if (other.tag == "Player")
            {
                Transform other_tf = other.gameObject.transform;
                PlayerStatus other_st = other_tf.GetComponent<PlayerStatus>();

                if (Vector3.Distance(other_st.transform.position, transform.position) 
                    <= Vector3.Distance(player_tf.transform.position, transform.position))
                {
                    Debug.Log(player_tf.gameObject.name);
                    player_tf = other_tf;
                    player_st = other_st;

                }
                    is_Tracking = true;
            }
    }
    //bool CheckDistance()
    //{
    //    float distance = Vector3.Distance(player.transform.position, transform.position);
    //    Debug.Log("몬스터와 캐릭터 사이의 거리" + distance);
    //    if (distance <= targetRange) return true;

    //    return false;
    //}
}
