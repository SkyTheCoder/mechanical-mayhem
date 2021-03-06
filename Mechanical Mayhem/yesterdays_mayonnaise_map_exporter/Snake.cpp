// Level: Skip.
// Map data automatically generated by Processing.
// Each pair of floats in an array is a coordinate for a tile, in tilemap space.

float staticSpikes[56] = {
	1.0f, 6.0f, 2.0f, 6.0f, 3.0f, 6.0f, 1.0f, 7.0f, 2.0f, 7.0f, 1.0f, 8.0f, 7.0f, 12.0f, 8.0f, 12.0f, 9.0f, 12.0f, 10.0f, 12.0f, 22.0f, 14.0f, 16.0f, 15.0f, 17.0f, 15.0f, 22.0f, 15.0f, 22.0f, 16.0f, 22.0f, 17.0f, 
	15.0f, 21.0f, 16.0f, 21.0f, 17.0f, 21.0f, 18.0f, 21.0f, 19.0f, 21.0f, 20.0f, 21.0f, 22.0f, 37.0f, 22.0f, 38.0f, 22.0f, 39.0f, 22.0f, 40.0f, 22.0f, 41.0f, 22.0f, 42.0f
};
float redSpikes[40] = {
	21.0f, 7.0f, 22.0f, 7.0f, 11.0f, 8.0f, 11.0f, 9.0f, 7.0f, 17.0f, 8.0f, 17.0f, 9.0f, 17.0f, 10.0f, 17.0f, 18.0f, 18.0f, 18.0f, 19.0f, 6.0f, 26.0f, 6.0f, 27.0f, 4.0f, 38.0f, 5.0f, 38.0f, 4.0f, 39.0f, 5.0f, 39.0f, 
	4.0f, 40.0f, 5.0f, 40.0f, 4.0f, 41.0f, 5.0f, 41.0f
};
float blueSpikes[20] = {
	13.0f, 6.0f, 14.0f, 6.0f, 13.0f, 7.0f, 14.0f, 7.0f, 15.0f, 18.0f, 15.0f, 19.0f, 2.0f, 28.0f, 3.0f, 28.0f, 14.0f, 30.0f, 14.0f, 31.0f
};
float abilities[16] = {
	2.0f, 12.0f, 1.0f, 16.0f, 10.0f, 16.0f, 22.0f, 19.0f, 1.0f, 27.0f, 19.0f, 31.0f, 4.0f, 36.0f, 18.0f, 36.0f
};

AddStaticSpikes(staticSpikes, 28);
AddRedSpikes(redSpikes, 20, redDimension);
AddBlueSpikes(blueSpikes, 10, blueDimension);
AddAbilities(abilities, 8);
