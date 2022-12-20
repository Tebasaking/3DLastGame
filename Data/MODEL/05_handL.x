xof 0302txt 0064
template Header {
 <3D82AB43-62DA-11cf-AB39-0020AF71E433>
 WORD major;
 WORD minor;
 DWORD flags;
}

template Vector {
 <3D82AB5E-62DA-11cf-AB39-0020AF71E433>
 FLOAT x;
 FLOAT y;
 FLOAT z;
}

template Coords2d {
 <F6F23F44-7686-11cf-8F52-0040333594A3>
 FLOAT u;
 FLOAT v;
}

template Matrix4x4 {
 <F6F23F45-7686-11cf-8F52-0040333594A3>
 array FLOAT matrix[16];
}

template ColorRGBA {
 <35FF44E0-6C7C-11cf-8F52-0040333594A3>
 FLOAT red;
 FLOAT green;
 FLOAT blue;
 FLOAT alpha;
}

template ColorRGB {
 <D3E16E81-7835-11cf-8F52-0040333594A3>
 FLOAT red;
 FLOAT green;
 FLOAT blue;
}

template IndexedColor {
 <1630B820-7842-11cf-8F52-0040333594A3>
 DWORD index;
 ColorRGBA indexColor;
}

template Boolean {
 <4885AE61-78E8-11cf-8F52-0040333594A3>
 WORD truefalse;
}

template Boolean2d {
 <4885AE63-78E8-11cf-8F52-0040333594A3>
 Boolean u;
 Boolean v;
}

template MaterialWrap {
 <4885AE60-78E8-11cf-8F52-0040333594A3>
 Boolean u;
 Boolean v;
}

template TextureFilename {
 <A42790E1-7810-11cf-8F52-0040333594A3>
 STRING filename;
}

template Material {
 <3D82AB4D-62DA-11cf-AB39-0020AF71E433>
 ColorRGBA faceColor;
 FLOAT power;
 ColorRGB specularColor;
 ColorRGB emissiveColor;
 [...]
}

template MeshFace {
 <3D82AB5F-62DA-11cf-AB39-0020AF71E433>
 DWORD nFaceVertexIndices;
 array DWORD faceVertexIndices[nFaceVertexIndices];
}

template MeshFaceWraps {
 <4885AE62-78E8-11cf-8F52-0040333594A3>
 DWORD nFaceWrapValues;
 Boolean2d faceWrapValues;
}

template MeshTextureCoords {
 <F6F23F40-7686-11cf-8F52-0040333594A3>
 DWORD nTextureCoords;
 array Coords2d textureCoords[nTextureCoords];
}

template MeshMaterialList {
 <F6F23F42-7686-11cf-8F52-0040333594A3>
 DWORD nMaterials;
 DWORD nFaceIndexes;
 array DWORD faceIndexes[nFaceIndexes];
 [Material]
}

template MeshNormals {
 <F6F23F43-7686-11cf-8F52-0040333594A3>
 DWORD nNormals;
 array Vector normals[nNormals];
 DWORD nFaceNormals;
 array MeshFace faceNormals[nFaceNormals];
}

template MeshVertexColors {
 <1630B821-7842-11cf-8F52-0040333594A3>
 DWORD nVertexColors;
 array IndexedColor vertexColors[nVertexColors];
}

template Mesh {
 <3D82AB44-62DA-11cf-AB39-0020AF71E433>
 DWORD nVertices;
 array Vector vertices[nVertices];
 DWORD nFaces;
 array MeshFace faces[nFaces];
 [...]
}

Header{
1;
0;
1;
}

Mesh {
 70;
 14.98839;-2.73854;2.77233;,
 15.02550;2.53965;-2.45354;,
 14.95885;-2.71215;-2.47956;,
 15.05504;2.51326;2.79836;,
 21.30657;-2.79256;-2.51568;,
 21.40282;2.43283;2.76224;,
 21.33613;-2.81895;2.73622;,
 21.37326;2.45922;-2.48965;,
 18.25318;3.26309;-3.22875;,
 21.30657;-2.79256;-2.51568;,
 18.16747;-3.50987;-3.26231;,
 21.37326;2.45922;-2.48965;,
 14.95885;-2.71215;-2.47956;,
 15.02550;2.53965;-2.45354;,
 18.29130;3.22906;3.54445;,
 21.37326;2.45922;-2.48965;,
 18.25318;3.26309;-3.22875;,
 21.40282;2.43283;2.76224;,
 15.02550;2.53965;-2.45354;,
 15.05504;2.51326;2.79836;,
 18.20558;-3.54392;3.51089;,
 21.40282;2.43283;2.76224;,
 18.29130;3.22906;3.54445;,
 21.33613;-2.81895;2.73622;,
 15.05504;2.51326;2.79836;,
 14.98839;-2.73854;2.77233;,
 18.16747;-3.50987;-3.26231;,
 21.33613;-2.81895;2.73622;,
 18.20558;-3.54392;3.51089;,
 21.30657;-2.79256;-2.51568;,
 0.13834;-4.29677;-4.72597;,
 14.08466;4.42223;-4.76061;,
 13.97176;-4.47172;-4.80467;,
 0.25123;4.59770;-4.68190;,
 0.26957;5.59271;-3.67645;,
 14.14739;5.37808;4.13914;,
 14.10299;5.41776;-3.75515;,
 0.31397;5.55303;4.21784;,
 0.30689;4.54797;5.21337;,
 14.02740;-4.52146;5.09060;,
 14.14030;4.37251;5.13466;,
 0.19401;-4.34650;5.16930;,
 14.97770;-4.48968;-3.80986;,
 15.13501;4.36511;4.12850;,
 15.02212;-4.52935;4.08442;,
 15.09059;4.40479;-3.76580;,
 14.10299;5.41776;-3.75515;,
 15.09059;4.40479;-3.76580;,
 14.14030;4.37251;5.13466;,
 14.14739;5.37808;4.13914;,
 0.25123;4.59770;-4.68190;,
 14.08466;4.42223;-4.76061;,
 14.14030;4.37251;5.13466;,
 0.30689;4.54797;5.21337;,
 14.97770;-4.48968;-3.80986;,
 15.09059;4.40479;-3.76580;,
 15.13501;4.36511;4.12850;,
 14.02740;-4.52146;5.09060;,
 0.19401;-4.34650;5.16930;,
 0.13834;-4.29677;-4.72597;,
 14.02740;-4.52146;5.09060;,
 15.02212;-4.52935;4.08442;,
 14.97770;-4.48968;-3.80986;,
 13.97176;-4.47172;-4.80467;,
 0.19401;-4.34650;5.16930;,
 0.30689;4.54797;5.21337;,
 0.13834;-4.29677;-4.72597;,
 0.25123;4.59770;-4.68190;,
 0.31397;5.55303;4.21784;,
 0.26957;5.59271;-3.67645;;
 
 48;
 3;0,1,2;,
 3;0,3,1;,
 3;4,5,6;,
 3;4,7,5;,
 3;8,9,10;,
 3;8,11,9;,
 3;8,12,13;,
 3;8,10,12;,
 3;14,15,16;,
 3;14,17,15;,
 3;14,18,19;,
 3;14,16,18;,
 3;20,21,22;,
 3;20,23,21;,
 3;20,24,25;,
 3;20,22,24;,
 3;26,27,28;,
 3;26,29,27;,
 3;26,0,2;,
 3;26,28,0;,
 3;30,31,32;,
 3;30,33,31;,
 3;34,35,36;,
 3;34,37,35;,
 3;38,39,40;,
 3;38,41,39;,
 3;42,43,44;,
 3;42,45,43;,
 3;46,47,31;,
 3;48,43,49;,
 3;36,50,34;,
 3;36,51,50;,
 3;52,37,53;,
 3;52,35,37;,
 3;54,31,47;,
 3;54,32,31;,
 3;55,35,56;,
 3;55,36,35;,
 3;43,57,44;,
 3;43,48,57;,
 3;58,59,60;,
 3;60,59,61;,
 3;61,59,62;,
 3;59,63,62;,
 3;64,65,66;,
 3;66,65,67;,
 3;65,68,67;,
 3;68,69,67;;
 
 MeshMaterialList {
  3;
  48;
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0;;
  Material {
   0.660000;0.660000;0.660000;1.000000;;
   15.000000;
   0.300000;0.300000;0.300000;;
   0.000000;0.000000;0.000000;;
   TextureFilename {
    "main.png";
   }
  }
  Material {
   0.340000;0.340000;0.340000;1.000000;;
   17.000000;
   0.250000;0.250000;0.250000;;
   0.000000;0.000000;0.000000;;
   TextureFilename {
    "sub.png";
   }
  }
  Material {
   0.800000;0.800000;0.800000;1.000000;;
   20.000000;
   0.400000;0.400000;0.400000;;
   0.000000;0.000000;0.000000;;
   TextureFilename {
    "sword.png";
   }
  }
 }
 MeshNormals {
  58;
  -0.999904;0.012661;0.005687;,
  -0.999904;0.012662;0.005688;,
  -0.999904;0.012660;0.005686;,
  0.999904;-0.012669;-0.005691;,
  0.999904;-0.012669;-0.005691;,
  0.999904;-0.012669;-0.005691;,
  -0.002158;0.004982;-0.999985;,
  -0.002150;0.004982;-0.999985;,
  0.016156;0.999857;0.004933;,
  0.009095;-0.005070;0.999946;,
  -0.005625;0.005026;-0.999972;,
  0.377477;-0.000204;-0.926019;,
  0.376498;0.243737;-0.893779;,
  0.001562;0.509779;-0.860304;,
  0.010842;0.968734;-0.247866;,
  0.300500;0.840502;-0.450839;,
  0.391102;0.886760;0.246365;,
  0.013789;0.860257;0.509674;,
  0.008656;0.247895;0.968748;,
  0.302865;0.445414;0.842543;,
  0.387860;-0.009489;0.921669;,
  0.005625;-0.005026;0.999972;,
  0.921650;-0.009776;-0.387900;,
  0.925950;-0.013622;0.377401;,
  0.898004;0.230844;0.374567;,
  0.846023;0.444135;-0.294939;,
  0.230974;0.001894;-0.972958;,
  0.230966;0.001889;-0.972960;,
  -0.235172;0.007796;-0.971923;,
  -0.235179;0.007801;-0.971921;,
  0.248726;0.968568;0.003467;,
  0.016155;0.999857;0.004933;,
  0.248726;0.968568;0.003468;,
  -0.217303;0.976085;0.006127;,
  -0.217302;0.976085;0.006126;,
  0.241913;-0.007874;0.970266;,
  0.009087;-0.005069;0.999946;,
  0.241923;-0.007880;0.970264;,
  -0.224224;-0.001987;0.974536;,
  -0.224218;-0.001984;0.974537;,
  -0.009216;-0.999945;-0.004974;,
  0.224156;-0.974534;-0.006159;,
  -0.009216;-0.999945;-0.004974;,
  0.224155;-0.974534;-0.006159;,
  -0.242080;-0.970250;-0.003514;,
  -0.242079;-0.970250;-0.003513;,
  -0.012674;-0.999907;-0.004954;,
  -0.012707;-0.999907;-0.004986;,
  -0.012730;-0.999907;-0.004875;,
  -0.012740;-0.999907;-0.004874;,
  -0.012684;-0.999907;-0.005096;,
  -0.012675;-0.999906;-0.005239;,
  -0.999904;0.012662;0.005689;,
  -0.999904;0.012670;0.005688;,
  -0.999904;0.012662;0.005688;,
  -0.999904;0.012677;0.005688;,
  -0.999903;0.012684;0.005688;,
  -0.999903;0.012683;0.005688;;
  48;
  3;0,0,1;,
  3;0,2,0;,
  3;3,3,4;,
  3;3,5,3;,
  3;6,26,7;,
  3;6,27,26;,
  3;6,28,29;,
  3;6,7,28;,
  3;8,30,31;,
  3;8,32,30;,
  3;8,33,34;,
  3;8,31,33;,
  3;9,35,36;,
  3;9,37,35;,
  3;9,38,39;,
  3;9,36,38;,
  3;40,41,42;,
  3;40,43,41;,
  3;40,44,45;,
  3;40,42,44;,
  3;10,12,11;,
  3;10,13,12;,
  3;14,16,15;,
  3;14,17,16;,
  3;18,20,19;,
  3;18,21,20;,
  3;22,24,23;,
  3;22,25,24;,
  3;15,25,12;,
  3;19,24,16;,
  3;15,13,14;,
  3;15,12,13;,
  3;19,17,18;,
  3;19,16,17;,
  3;22,12,25;,
  3;22,11,12;,
  3;25,16,24;,
  3;25,15,16;,
  3;24,20,23;,
  3;24,19,20;,
  3;46,47,48;,
  3;48,47,49;,
  3;49,47,50;,
  3;47,51,50;,
  3;52,53,54;,
  3;54,53,55;,
  3;53,56,55;,
  3;56,57,55;;
 }
 MeshTextureCoords {
  70;
  0.757640;0.097620;,
  0.813850;0.055250;,
  0.814140;0.097550;,
  0.757350;0.055320;,
  0.894250;0.034950;,
  0.950650;0.091460;,
  0.894340;0.091430;,
  0.950560;0.034980;,
  0.139700;0.050120;,
  0.173380;0.113770;,
  0.139610;0.122620;,
  0.173450;0.057550;,
  0.105100;0.113560;,
  0.105180;0.057340;,
  0.300010;0.030350;,
  0.323030;0.096320;,
  0.300080;0.103100;,
  0.322970;0.039900;,
  0.266930;0.094250;,
  0.266880;0.037840;,
  0.048990;0.055210;,
  0.082990;0.119870;,
  0.049200;0.128070;,
  0.082830;0.063380;,
  0.014720;0.120080;,
  0.014560;0.063590;,
  0.822220;0.126650;,
  0.757360;0.142890;,
  0.749360;0.126740;,
  0.813860;0.142830;,
  0.527910;0.379620;,
  0.447330;0.253480;,
  0.528300;0.253730;,
  0.446940;0.379370;,
  0.659050;0.287890;,
  0.731290;0.413600;,
  0.659430;0.413820;,
  0.730910;0.287670;,
  0.771310;0.143020;,
  0.852660;0.268700;,
  0.771690;0.268950;,
  0.852270;0.142770;,
  0.919630;0.739400;,
  0.847570;0.820150;,
  0.847810;0.739180;,
  0.919380;0.820370;,
  0.438220;0.253220;,
  0.447360;0.244150;,
  0.838140;0.820120;,
  0.847220;0.829250;,
  0.649940;0.287940;,
  0.650330;0.413860;,
  0.740400;0.413590;,
  0.740010;0.287660;,
  0.528320;0.244400;,
  0.659460;0.422940;,
  0.731320;0.422720;,
  0.838390;0.739150;,
  0.650330;0.441660;,
  0.740400;0.441930;,
  0.649940;0.567580;,
  0.659020;0.576720;,
  0.730880;0.576940;,
  0.740010;0.567860;,
  0.908320;0.861000;,
  0.908080;0.941970;,
  0.818280;0.860720;,
  0.818040;0.941690;,
  0.898940;0.951040;,
  0.827110;0.950820;;
 }
}
