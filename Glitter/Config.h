#pragma once

#include "Singleton.h"



class Config : public Singleton<Config>
{
	friend class Singleton<Config>;
public:
	int SCR_WIDTH = 1920;
	int SCR_HEIGHT = 900;

	vector<string> cubemapFaces
	{
		"./resources/textures/skybox/hw_nightsky/nightsky_rt.tga",
		"./resources/textures/skybox/hw_nightsky/nightsky_lf.tga",
		"./resources/textures/skybox/hw_nightsky/nightsky_up.tga",
		"./resources/textures/skybox/hw_nightsky/nightsky_dn.tga",
		"./resources/textures/skybox/hw_nightsky/nightsky_bk.tga",
		"./resources/textures/skybox/hw_nightsky/nightsky_ft.tga"
		//"./resources/textures/skybox/mp_depression/depression-pass_rt.tga",
		//"./resources/textures/skybox/mp_depression/depression-pass_lf.tga",
		//"./resources/textures/skybox/mp_depression/depression-pass_up.tga",
		//"./resources/textures/skybox/mp_depression/depression-pass_dn.tga",
		//"./resources/textures/skybox/mp_depression/depression-pass_bk.tga",
		//"./resources/textures/skybox/mp_depression/depression-pass_ft.tga"
		//"./resources/textures/skybox/tuto/right.jpg",
		//"./resources/textures/skybox/tuto/left.jpg",
		//"./resources/textures/skybox/tuto/top.jpg",
		//"./resources/textures/skybox/tuto/bottom.jpg",
		//"./resources/textures/skybox/tuto/back.jpg",
		//"./resources/textures/skybox/tuto/front.jpg"
	};

	//var.set("win_posx", 150);
	//var.set("win_posy", 150);
	//var.set("win_width", 800);
	//var.set("win_height", 600);

	//var.set("mouse_sensivity", 0.005f);

	//var.set("cam_znear", 0.1f);
	//var.set("cam_zfar", 800.0f);
	//var.set("cam_fovy", 60.0f);

	//var.set("cam_anaglyph_offset", 0.16f);

	//var.set("enable_anaglyph", false);
	//var.set("enable_move_control", true);
	//var.set("enable_effects", true);
	//var.set("enable_vignette", false);
	//var.set("enable_bloom", true);
	//var.set("enable_noise", false);
	//var.set("enable_pdc", false);
	//var.set("enable_underwater", false);

	//var.set("terrain_chunks_drawn", 0);
	//var.set("terrain_chunks_reflected_drawn", 0);

	//var.set("show_camera_splines", false);

	//var.set("time_speed", 1.0f);

	//var.set("water_height", 4.2f);
};