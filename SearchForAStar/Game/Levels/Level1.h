#pragma once

using SFAS::Game::Level;

#include <d3dx9.h>

Level getLevel1() {
	Level::Enemy* level1Enemies = static_cast<Level::Enemy*> 
			(::operator new (sizeof(Level::Enemy[4])));
	level1Enemies[0] = Level::Enemy (
		D3DXVECTOR3(100,100,0)
	);
	level1Enemies[1] = Level::Enemy (
		D3DXVECTOR3(200,500,0)
	);
	level1Enemies[2] = Level::Enemy (
		D3DXVECTOR3(600,300,0)
	);
	level1Enemies[3] = Level::Enemy (
		D3DXVECTOR3(300,500,0)
	);


	Level level1(
		"L0",
		"Level 0",
		4,
		level1Enemies,
		L"textures/skybox.png"
	);

	return level1;
}
