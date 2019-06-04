String mapName = "Cavern";

color staticColor = color(0, 0, 0, 255);
color staticSpikeColor = color(255, 0, 246, 255);
color redColor = color(255, 0, 8, 255);
color redSpikeColor = color(255, 136, 0, 255);
color blueColor = color(0, 12, 255, 255);
color blueSpikeColor = color(0, 225, 255, 255);
color chipColor = color(255, 233, 0, 255);

void setup()
{
  PImage image = loadImage(mapName + ".png");
  String staticData = "";
  String redData = "";
  String blueData = "";
  String cpp = "// Level: " + mapName + ".\r\n// Map data automatically generated by Processing.\r\n// Each pair of floats in an array is a coordinate for a tile, in tilemap space.\r\n\r\n";
  FloatList staticSpikes = new FloatList();
  FloatList redSpikes = new FloatList();
  FloatList blueSpikes = new FloatList();
  FloatList abilities = new FloatList();
  for (int y = 0; y < image.height; y++)
  {
    for (int x = 0; x < image.width; x++)
    {
      color c = image.get(x, y);
      
      int staticValue = 0;
      int redValue = 0;
      int blueValue = 0;
      
      if (c == staticColor)
      {
        staticValue = 1;
      }
      else if (c == redColor)
      {
        redValue = 2;
      }
      else if (c == blueColor)
      {
        blueValue = 3;
      }
      else if (c == staticSpikeColor)
      {
        staticSpikes.push(x);
        staticSpikes.push(y);
      }
      else if (c == redSpikeColor)
      {
        redSpikes.push(x);
        redSpikes.push(y);
      }
      else if (c == blueSpikeColor)
      {
        blueSpikes.push(x);
        blueSpikes.push(y);
      }
      else if (c == chipColor)
      {
        abilities.push(x);
        abilities.push(y);
      }
      
      staticData += staticValue + " ";
      redData += redValue + " ";
      blueData += blueValue + " ";
    }
    
    staticData += "\r\n";
    redData += "\r\n";
    blueData += "\r\n";
  }
  
  if (staticSpikes.size() > 0)
  {
    cpp += "float staticSpikes[" + staticSpikes.size() + "] = {";
    for (int i = 0; i < staticSpikes.size(); i++)
    {
      if (i % 32 == 0)
        cpp += "\r\n\t";
      cpp += staticSpikes.get(i) + "f";
      if (i != staticSpikes.size() - 1)
        cpp += ", ";
    }
    cpp += "\r\n};\r\n";
  }
  
  if (redSpikes.size() > 0)
  {
    cpp += "float redSpikes[" + redSpikes.size() + "] = {";
    for (int i = 0; i < redSpikes.size(); i++)
    {
      if (i % 32 == 0)
        cpp += "\r\n\t";
      cpp += redSpikes.get(i) + "f";
      if (i != redSpikes.size() - 1)
        cpp += ", ";
    }
    cpp += "\r\n};\r\n";
  }
  
  if (blueSpikes.size() > 0)
  {
    cpp += "float blueSpikes[" + blueSpikes.size() + "] = {";
    for (int i = 0; i < blueSpikes.size(); i++)
    {
      if (i % 32 == 0)
        cpp += "\r\n\t";
      cpp += blueSpikes.get(i) + "f";
      if (i != blueSpikes.size() - 1)
        cpp += ", ";
    }
    cpp += "\r\n};\r\n";
  }
  
  if (abilities.size() > 0)
  {
    cpp += "float abilities[" + abilities.size() + "] = {";
    for (int i = 0; i < abilities.size(); i++)
    {
      if (i % 32 == 0)
        cpp += "\r\n\t";
      cpp += abilities.get(i) + "f";
      if (i != abilities.size() - 1)
        cpp += ", ";
    }
    cpp += "\r\n};\r\n";
  }
  
  cpp += "\r\n";
  
  if (staticSpikes.size() > 0)
  {
    cpp += "AddStaticSpikes(staticSpikes, " + (staticSpikes.size() / 2) + ");\r\n";
  }
  
  if (redSpikes.size() > 0)
  {
    cpp += "AddRedSpikes(redSpikes, " + (redSpikes.size() / 2) + ", redDimension);\r\n";
  }
  
  if (blueSpikes.size() > 0)
  {
    cpp += "AddBlueSpikes(blueSpikes, " + (blueSpikes.size() / 2) + ", blueDimension);\r\n";
  }
  
  if (abilities.size() > 0)
  {
    cpp += "AddAbilities(abilities, " + (abilities.size() / 2) + ");\r\n";
  }
  
  PrintWriter staticOutput = createWriter(mapName + "Static.txt");
  staticOutput.println("width " + image.width);
  staticOutput.println("height " + image.height);
  staticOutput.println("data");
  staticOutput.print(staticData);
  staticOutput.flush();
  staticOutput.close();
  
  PrintWriter redOutput = createWriter(mapName + "Red.txt");
  redOutput.println("width " + image.width);
  redOutput.println("height " + image.height);
  redOutput.println("data");
  redOutput.print(redData);
  redOutput.flush();
  redOutput.close();
  
  PrintWriter blueOutput = createWriter(mapName + "Blue.txt");
  blueOutput.println("width " + image.width);
  blueOutput.println("height " + image.height);
  blueOutput.println("data");
  blueOutput.print(blueData);
  blueOutput.flush();
  blueOutput.close();
  
  PrintWriter cppOutput = createWriter(mapName + ".cpp");
  cppOutput.print(cpp);
  cppOutput.flush();
  cppOutput.close();
  
  println("Done!");
}

void draw()
{
  exit();
}
