#include "stdafx.h"

//#include <fstream>
//#include <sstream>
//#include <vector>
//
//#include "obj.h"
//#include "parser.h"
//#include "stringutils.h"

using std::string;
using std::stringstream;
using std::vector;

//_____________________________________________________________________________
OBJ::OBJ(void)
{
}


//_____________________________________________________________________________
//OBJ::OBJ(const std::string& filename) : _filename(filename){}


//_____________________________________________________________________________
OBJ::~OBJ(void)
{
}


//_____________________________________________________________________________
bool
OBJ::exVertices(const char* line, Mesh& mesh) const
{
  static size_t index = 0;
  stringstream ss(line);
  string command = "";
  ss >> command;

  //Checke ob die Zeile mit "v" beginnt
  if (command != "v" )
    return false;

  double coords[3];
  index++;
  ss >> coords[0]; ss >> coords[1]; ss >> coords[2];
  mesh.vertices.push_back(vector<double>(coords, coords + 3) );
  //mesh.indices.push_back(index);
  mesh.numVertices++;

  return true;
}


//_____________________________________________________________________________
bool
OBJ::exFaces(const char* line, Mesh& mesh)
const
{
  vector<size_t> pointIdx;
  vector<size_t> textureIdx;
  vector<size_t> normalIdx;
  stringstream ss(line);
  string command = "";
  string content = "";
  ss >> command;

  //Checke ob die Zeile mit "f" beginnt
  if (command != "f")
    return false;

  // TODO hier besteht das Problem, dass bei nachfolgenden whitespaces in
  // der obj datei, 4 elemente in den faces vector gespeichert werden
  for (int i=0; i<3; i++)
  {
    
    vector<string> fields;
    
    ss >> content;
    split(fields, content, '/');

    //fields[0] = Point index
    //fields[1] = TextureCoord index
    //fields[2] = VertexNormal index;
    size_t idx;
    stringstream tmp(fields[0]);
    tmp >> idx;
    pointIdx.push_back(idx-1);


    if (fields.size() >= 2 && fields[1] != "")
    {
      tmp = stringstream(fields[1]);
      tmp >> idx;
      textureIdx.push_back(idx-1);
    }


    if (fields.size() >= 3 && fields[2] != "")
    {
      tmp = stringstream(fields[2]);
      tmp >> idx;
      normalIdx.push_back(idx-1);
    }

  }
 
  //Fülle vertexFaces
  for (size_t i=0; i<pointIdx.size(); i++)
  {
    mesh.vertexFaces[pointIdx[i] ].push_back(mesh.faces.size() );
  }

  //Fülle fileNormalsIdx
  map<size_t, size_t> tmpMap;
  for (size_t i=0; i<normalIdx.size(); i++)
  {
    tmpMap[pointIdx[i] ] = normalIdx[i];
  }
  mesh.fileNormalsIdx.push_back(tmpMap);


  
  mesh.faces.push_back(pointIdx);
  mesh.numTriangles++;

  return true;
}


//_____________________________________________________________________________
bool
OBJ::exTextureCoord(const char* line, Mesh& mesh) const
{
  //TODO: Nicht ausimplementiert... kann später erweitert werden
  stringstream ss(line);
  return false;
}


//_____________________________________________________________________________
bool
OBJ::exNormals(const char* line, Mesh& mesh)
{
  stringstream ss(line);
  string command = "";
  ss >> command;

  //Checke ob die Zeile mit "vn" beginnt
  if (command != "vn")
    return false;

  
  double x, y, z;
  ss >> x; ss >> y; ss >> z;
  mesh.normals.push_back(Normal(x, y, z) );
  mesh.normalType = FILENORMAL;


  return true;
}


//_____________________________________________________________________________
bool
OBJ::exComment(const char* line, Mesh& mesh) const
{
  stringstream ss(line);
  string command;
  ss >> command;
  if (command[0] == '#' || command[0] == '\n')
    return true;

  return false;
}


//_____________________________________________________________________________
bool
OBJ::exGroup(const char* line, Mesh& mesh) const
{
  stringstream ss(line);
  string command;
  ss >> command;
  if (command[0] != 'g')
    return false;

  WOUT(_T("Die \"Group\" Funktionalität ist in \"obj.cpp\" noch nicht ")
       _T("implementiert.") );
  return true;
}


//_____________________________________________________________________________
void
OBJ::fillNormals(Mesh& mesh) const
{
  size_t face;
  size_t nVertices = mesh.vertices.size();
  vector<double> pointCoord;
  Point p0, p1, p2;
  Vector normal = Vector(0,0,0);

  // Durchlaufe alle Punkte
  for (size_t i=0; i<nVertices; i++)
  {
    normal = Vector(0,0,0);

    // Durchlaufe alle am Punkt angrenzenden Seiten
    for (size_t j=0; j<mesh.vertexFaces[i].size(); j++)
    {
      face = mesh.vertexFaces[i][j];

      pointCoord = mesh.vertices[mesh.faces[face][0] ];
      p0.x = pointCoord[0];
      p0.y = pointCoord[1];
      p0.z = pointCoord[2];

      pointCoord = mesh.vertices[mesh.faces[face][1] ];
      p1.x = pointCoord[0];
      p1.y = pointCoord[1];
      p1.z = pointCoord[2];

      pointCoord = mesh.vertices[mesh.faces[face][2] ];
      p2.x = pointCoord[0];
      p2.y = pointCoord[1];
      p2.z = pointCoord[2];

      // Berechne Normale
      normal += cross(p1 - p0, p2 - p0);

    }

    mesh.normals.push_back(normal.norm() );
  }
}


//_____________________________________________________________________________
MeshParser*
OBJ::clone() const
{
  MeshParser* meshParser = new OBJ(*this);
  GBin.mrkDel(meshParser);

  return meshParser;
}


//_____________________________________________________________________________
bool 
OBJ::fill(Mesh& mesh)
{
  int length;
  int nLines = 1;
  char line[128];
  char * buffer;
  std::stringstream ss;
  std::ifstream ifs(_filename, std::ifstream::in);
  if (!ifs.is_open() )
  {
    EOUT("die Datei \"" << _filename << "\" konnte nicht geöffnet werden.");
    std::cin.get();
    exit(1);
  }

  // get length of file
  ifs.seekg(0, std::ios::end);
  length = static_cast<int>(ifs.tellg() );
  ifs.seekg(0, std::ios::beg);

  // allocate memory
  buffer = new char [length];

  // read data as a block
  ifs.read(buffer, length);
  ifs.close();

  // count '\n' in buffer to get number of lines in file
  for (size_t i=0; buffer[i]!='\0'; i++)
    if (buffer[i] == '\n')
      nLines++;

   ss << buffer;
  delete[] buffer;

  mesh.vertices.reserve(nLines);
  //mesh.indices.reserve(nLines);
  mesh.faces.reserve(nLines);
  mesh.normals.reserve(nLines);
  mesh.vertexFaces = vector<vector<size_t> >(nLines);
  mesh.fileNormalsIdx.reserve(nLines);
  
 

  while (ss.getline(line, 128) )
  {
    if (ss.eof() )
      break;

    bool valid = false;

    valid |= exVertices(line, mesh);
    if (valid) continue;
    valid |= exNormals(line, mesh);
    if (valid) continue;
    valid |= exFaces(line, mesh);
    if (valid) continue;
    valid |= (string(line) == "");
    if (valid) continue;
    valid |= exComment(line, mesh);
    if (valid) continue;
    valid |= exGroup(line, mesh);
    if (valid) continue;
    valid |= exTextureCoord(line, mesh);
        
    
    if(!valid)
    {
      return false;
    }
  }

  if (mesh.normals.size() == 0)
    fillNormals(mesh);

  mesh.vertices.shrink_to_fit();
  //mesh.indices.shrink_to_fit();
  mesh.faces.shrink_to_fit();
  mesh.normals.shrink_to_fit();
  mesh.fileNormalsIdx.shrink_to_fit();
  //TODO der Vector enthält viele Zellen die nicht belegt sind. shrink_to_fit()
  //funktioniert hier nicht, da schon Instanzen erzeugt wurden.
  //mesh.vertexFaces.shrink_to_fit();

  

  return true;
}