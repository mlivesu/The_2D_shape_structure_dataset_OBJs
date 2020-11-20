#include <cinolib/string_utilities.h>
#include <cinolib/io/write_OBJ.h>
#include <cinolib/meshes/trimesh.h>
#include <fstream>
#include <sstream>

using namespace cinolib;

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

int main(int /*argc*/, char **argv)
{
    std::vector<double> verts;
    std::vector<uint>   tris;

    std::ifstream f(argv[1]);
    if(f.is_open())
    {
        bool read_points = true;
        std::string line;
        std::getline(f,line); // reads # points
        while(std::getline(f,line))
        {
            if(line[0]=='#')
            {
                read_points = false;
                // remove the last point. It's an (unused) copy of the first one....
                verts.pop_back();
                verts.pop_back();
                verts.pop_back();
            }
            else
            {
                std::stringstream ss(line);
                if(read_points)
                {
                    double x,y;
                    ss >> x >> y;
                    //std::cout << "v" << x << " " << y << std::endl;
                    verts.push_back(x);
                    verts.push_back(y);
                    verts.push_back(0);
                }
                else
                {
                    uint i,j,k;
                    ss >> i >> j >> k;
                    //std::cout << "t " << i << " " << j << " " << k << std::endl;
                    tris.push_back(i);
                    tris.push_back(k);
                    tris.push_back(j);
                }
            }
        }
        f.close();

        // for some reason some vertices are unused. just remove them
        Trimesh<> m(verts,tris);
        std::vector<uint> to_remove;
        for(uint vid=0; vid<m.num_verts(); ++vid)
        {
            if(m.vert_valence(vid)==0) to_remove.push_back(vid);
        }
        for(auto it=to_remove.rbegin(); it!=to_remove.rend(); ++it)
        {
            m.vert_remove_unreferenced(*it);
        }

        std::string s(argv[1]);
        write_OBJ((get_file_path(s) + get_file_name(s,false) + ".obj").c_str(), verts, tris, {});
    }

    return 0;
}
