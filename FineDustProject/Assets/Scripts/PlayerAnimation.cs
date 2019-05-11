﻿using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class PlayerAnimation : MonoBehaviour
{
    PlayerStatus player;
    Animator animator;
    CameraMgr CMgr;

    int anitype = 0;

    // Start is called before the first frame update
    void Start()
    {
        player = GetComponent<PlayerStatus>();
        animator = GetComponent<Animator>();
        CMgr = GetComponent<CameraMgr>();
    }

    // Update is called once per frame
    void Update()
    {
        MoveAni();
    }

    void MoveAni()
    {


        if (player.Ani_State_Walk_Run == PlayerStatus.ANI_TYPE.IDEL) anitype = (int)PlayerStatus.ANI_TYPE.IDEL;
        else if (player.Ani_State_Walk_Run == PlayerStatus.ANI_TYPE.WALK) anitype = (int)PlayerStatus.ANI_TYPE.WALK;
        else if (player.Ani_State_Walk_Run == PlayerStatus.ANI_TYPE.RUN) anitype = (int)PlayerStatus.ANI_TYPE.RUN;
        else animator.SetInteger("Ani_type", 0);

        if (CMgr.Camera_Num == 2 && anitype != 0)
            anitype += 2;

        animator.SetInteger("Ani_type", anitype);

        // 3인칭 카메라만 방향변화
        if (CMgr.Camera_Num == 0)
        {
            animator.SetFloat("DirectionX", player.Direction_X);
            animator.SetFloat("DirectionZ", player.Direction_Z);
        }


    }
}
