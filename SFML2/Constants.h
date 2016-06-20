#pragma once
#define APP_FPS 60
#define APP_WIDTH 1024
#define APP_HEIGHT 768
#define APP_TITLE "LittleBattle"
#define DEG2RAD 3.14159f / 180.0f

enum Action
{Move=0, Shot, MainMenu, Pause, TakeDamage, UseSkill, SetEffect, AllPlayers, AllBullets, Kill, SpawnPlayer, SpawnMonster, SetID};

enum Skill
{ArmorLow=0, ArmorMiddle, ArmorHigh};

enum Effect
{DOT=0};