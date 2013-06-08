#pragma once

using SFAS::Game::Level;

#include <d3dx9.h>

Level getLevel1() {
	Level::EnemyType* enemyTypes = new Level::EnemyType[2];
	enemyTypes[0].id = 0;
	enemyTypes[0].aiType = "suicide";
	enemyTypes[0].weapon = "none";
	enemyTypes[0].dimensions = D3DXVECTOR3(20.0f, 20.0f, 0.0f);
	enemyTypes[0].textureFile = L"textures/enemy.png";

	enemyTypes[1].id = 0;
	enemyTypes[1].aiType = "avoid";
	enemyTypes[1].weapon = "laser";
	enemyTypes[1].dimensions = D3DXVECTOR3(30.0f, 30.0f, 0.0f);
	enemyTypes[1].textureFile = L"textures/enemy2.png";
	

	Level::Enemy* level1Enemies = static_cast<Level::Enemy*> 
			(::operator new (sizeof(Level::Enemy[4])));
	level1Enemies[0] = Level::Enemy (
		0, D3DXVECTOR3(100,100,0)
	);
	level1Enemies[1] = Level::Enemy (
		0, D3DXVECTOR3(200,500,0)
	);
	level1Enemies[2] = Level::Enemy (
		0, D3DXVECTOR3(600,300,0)
	);
	level1Enemies[3] = Level::Enemy (
		0, D3DXVECTOR3(300,500,0)
	);
	level1Enemies[4] = Level::Enemy (
		1, D3DXVECTOR3(485,500,0)
	);

	Level level1;
	
	level1.id   = "L0";
	level1.name = "Level 0";
	level1.enemyCount = 5;
	level1.enemies = level1Enemies;
	level1.skyboxTextureFile = L"textures/skybox.png";
	level1.enemyTypeCount = 1;
	level1.enemyTypes = enemyTypes;

	return level1;
}
