﻿using System.Collections;
using System.Collections.Generic;

namespace Game.Protocol
{
    public class Protocol
    {
        public static int SC_ID = 1;
        public static int 	SC_PUT_PLAYER =			2;
        public static int  SC_REMOVE_PLAYER =		3;
        public static int  SC_PLAYER_STATUS	 =	4;
        public static int  SC_ALL_PLAYER_DATA =	5;
        public static int  SC_PUT_MONSTER =			6;
        public static int  SC_REMOVE_MONSTER = 7;

        public static int CS_INFO = 1;
        public static int CS_GET_ITEM = 2;
        public static int CS_MONSTER_HP = 3;

        public static int MAX_USER = 5;   // 0번째는 버림

        public Protocol()
        {

        }
    }
}