#pragma once
#define APP_FPS 900
#define APP_WIDTH 1024
#define APP_HEIGHT 768
#define APP_TITLE "LittleBattle"
#define DEG2RAD 3.14159f / 180.0f

enum Action
{Move=0, Shot, MainMenu, Pause, TakeDamage, UseSkill, SetEffect, AllPlayers, AllBullets, Kill, SpawnPlayer, SpawnMonster, SetID};

enum Skill
{	ShieldWall=0, Cure, Runner, Quad, Melee};

enum BuffType
{
	Armor = 0, Health, Speed, Damage, FireRate
};