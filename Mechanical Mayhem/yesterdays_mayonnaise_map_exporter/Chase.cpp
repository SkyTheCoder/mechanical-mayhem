// Level: Chase.
// Map data automatically generated by Processing.
// Each pair of floats in an array is a coordinate for a tile, in tilemap space.

float staticSpikes[62] = {
	0.0f, 15.0f, 1.0f, 15.0f, 2.0f, 15.0f, 3.0f, 15.0f, 0.0f, 16.0f, 1.0f, 16.0f, 2.0f, 16.0f, 14.0f, 16.0f, 15.0f, 16.0f, 16.0f, 16.0f, 17.0f, 16.0f, 18.0f, 16.0f, 19.0f, 16.0f, 0.0f, 17.0f, 1.0f, 17.0f, 0.0f, 18.0f, 
	0.0f, 19.0f, 0.0f, 20.0f, 1.0f, 28.0f, 2.0f, 28.0f, 3.0f, 28.0f, 4.0f, 28.0f, 0.0f, 36.0f, 0.0f, 37.0f, 0.0f, 38.0f, 0.0f, 39.0f, 0.0f, 40.0f, 0.0f, 43.0f, 0.0f, 44.0f, 0.0f, 45.0f, 0.0f, 46.0f
};

for (int i = 0; i < 62; i += 2)
{
	GameObject* spike = new GameObject(*objectManager.GetArchetypeByName("StaticSpike"));
	static_cast<Transform*>(spike->GetComponent("Transform"))->SetTranslation(Vector2D(staticSpikes[i] * 100.0f, staticSpikes[i + 1] * -100.0f));
	objectManager.AddObject(*spike);
}

float redSpikes[14] = {
	18.0f, 2.0f, 19.0f, 3.0f, 13.0f, 10.0f, 14.0f, 11.0f, 6.0f, 21.0f, 7.0f, 21.0f, 8.0f, 21.0f
};

for (int i = 0; i < 14; i += 2)
{
	GameObject* spike = new GameObject(*objectManager.GetArchetypeByName("RedSpike"));
	static_cast<Transform*>(spike->GetComponent("Transform"))->SetTranslation(Vector2D(redSpikes[i] * 100.0f, redSpikes[i + 1] * -100.0f));
	dimensionController.AddSpikeToDimension(redDimension, spike);
	objectManager.AddObject(*spike);
}

float blueSpikes[12] = {
	20.0f, 10.0f, 19.0f, 11.0f, 12.0f, 27.0f, 13.0f, 27.0f, 14.0f, 27.0f, 15.0f, 27.0f
};

for (int i = 0; i < 12; i += 2)
{
	GameObject* spike = new GameObject(*objectManager.GetArchetypeByName("BlueSpike"));
	static_cast<Transform*>(spike->GetComponent("Transform"))->SetTranslation(Vector2D(blueSpikes[i] * 100.0f, blueSpikes[i + 1] * -100.0f));
	dimensionController.AddSpikeToDimension(blueDimension, spike);
	objectManager.AddObject(*spike);
}

float chipsSpawns[10] = {
	2.0f, 12.0f, 11.0f, 21.0f, 3.0f, 27.0f, 13.0f, 35.0f, 7.0f, 38.0f
};

for (int i = 0; i < 10; i += 2)
{
	GameObject* chips = new GameObject(*objectManager.GetArchetypeByName("Collectible"));
	static_cast<Transform*>(chips->GetComponent("Transform"))->SetTranslation(Vector2D(chipsSpawns[i] * 100.0f, chipsSpawns[i + 1] * -100.0f));
	objectManager.AddObject(*chips);
}

