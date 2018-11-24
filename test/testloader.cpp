#include <assert.h>
#include <iostream>
#include <string>

#include "sceneloader.hpp"

void test_pop();

void test_read_file();
void test_load_scene();

void test_line_to_vec3();
void test_line_to_single();

void test_parse_camera();
void test_parse_sphere();
void test_parse_plane();
//void test_parse_mesh();
void test_parse_light();

int main()
{
    std::cout << "Testing pop... ";
    test_pop();
    std::cout << "PASS" << std::endl;

    std::cout << "Testing read_file()... ";
    test_read_file();
    std::cout << "PASS" << std::endl;

    std::cout << "Testing line_to_vec3()... ";
    test_line_to_vec3();
    std::cout << "PASS" << std::endl;

    std::cout << "Testing line_to_single()... ";
    test_line_to_single();
    std::cout << "PASS" << std::endl;

    /*
    std::cout << "Testing load_scene()... ";
    test_load_scene();
    std::cout << "PASS" << std::endl;
    */

    std::cout << "Testing parse_camera()... ";
    test_parse_camera();
    std::cout << "PASS" << std::endl;

    std::cout << "Testing parse_plane()... ";
    test_parse_plane();
    std::cout << "PASS" << std::endl;

    std::cout << "Testing parse_sphere()... ";
    test_parse_sphere();
    std::cout << "PASS" << std::endl;

    std::cout << "Testing parse_light()... ";
    test_parse_light();
    std::cout << "PASS" << std::endl;

    return 0;
}


void test_pop()
{
    std::deque<int> d { 1, 2, 3, 4, 5 };
    int exp[] { 1, 2, 3, 4 };

    for (int i = 0; i <= 3; i++){
        assert (exp[i] == pop(d));
    }

    assert (1 == d.size());
}


void test_read_file()
{
    std::string valid_file, invalid_file;
    valid_file = "../../test/testfile.txt";
    invalid_file = "NOT_A_FILE";

    std::vector<std::string> file_list;

    unsigned int exp_length = 4;
    std::vector<std::string> exp_contents {
        "1",
        "two",
        "line three",
        "4"
    };

    file_list = read_file(valid_file);

    // Check it read all the lines in the file
    assert (exp_length == file_list.size());

    // Check the lines are formatted properly
    for (unsigned int i = 0; i < file_list.size(); i++)
        assert (exp_contents[i] == file_list[i]);


    // Invalid files should return a 0-length vector
    exp_length = 0;
    file_list = read_file(invalid_file);
    assert(exp_length == file_list.size());

}


void test_line_to_vec3()
{
    std::string line_prefix { "pre" };
    std::string val_line { "pre 1.0 2.1 3.2" };
    Vec3 exp_v = Vec3 { 1.0, 2.1, 3.2 };

    assert (exp_v == line_to_vec3(val_line, line_prefix));

    int NUM_TEST_CASES = 5;
    std::string inv_lines[] {
        { "pre 1.0 2.1" },
        { "pre 1.0 a 3.2" },
        { "1.0 2.1 3.2" },
        { "1.0 2.1" },
        { "1.0 a 3.2"}
    };

    Vec3 v;
    bool inst_failed;
    for (int i = 0; i < NUM_TEST_CASES; i++)
    {
        inst_failed = false;
        try { v = line_to_vec3(inv_lines[i], line_prefix); }
        catch (const std::invalid_argument &e){ inst_failed = true; }
        assert (inst_failed);
    }

}


void test_line_to_single()
{
    std::string line_prefix { "pre" };

    assert (1 == line_to_single<int>("pre 1", line_prefix));
    assert ((2.1 - line_to_single<float>("pre 2.1", line_prefix)) < 0.01);
    assert ('a' == line_to_single<char>("pre a", line_prefix));
    assert ("str" == line_to_single<std::string>("pre str", line_prefix));

    bool inst_failed = false;
    try 
    { 
        int i = line_to_single<int>("1", line_prefix); 
    }
    catch (const std::invalid_argument &e){ inst_failed = true; }
    assert (inst_failed);


    inst_failed = false;
    try 
    { 
        float c = line_to_single<float>("pre str", line_prefix); 
    }
    catch (const std::invalid_argument &e){ inst_failed = true; }
    assert (inst_failed);
}


void test_parse_camera()
{
    std::shared_ptr<Camera> c;

    std::deque<std::string> val_camera {
        "pos: 1.0 2.0 3.0",
        "fov: 60", "f: 1000", "a: 1.33"
    };

    c = parse_camera(val_camera);
    assert (c->pos == (Vec3 { 1.0, 2.0, 3.0 }));
    assert (c->fov == 60);
    assert (c->f == 1000);
    assert ((c->a - 1.33) < 0.01);

    std::deque<std::string> inv_pos_field {
        "WRONG: 1.0 2.0 3.0",
        "fov: 60", "f: 1000", "a: 1.33"
    };

    std::deque<std::string> inv_pos_val {
        "pos: 1.0 3.0",
        "fov: 60", "f: 1000", "a: 1.33"
    };

    std::deque<std::string> inv_fov_field {
        "pos: 1.0 2.0 3.0",
        "WRONG: 60", "f: 1000", "a: 1.33"
    };
 
    std::deque<std::string> inv_fov_val {
        "pos: 1.0 2.0 3.0",
        "fov: fov", "f: 1000", "a: 1.33"
    };
 
    std::deque<std::string> inv_f_field {
        "pos: 1.0 2.0 3.0",
        "fov: 60", "WRONG: 1000", "a: 1.33"
    };

    std::deque<std::string> inv_f_val {
        "pos: 1.0 2.0 3.0",
        "fov: 60", "f: ", "a: 1.33"
    };

    std::deque<std::string> inv_a_field {
        "pos: 1.0 2.0 3.0",
        "fov: 60", "f: 1000", "WRONG: 1.33"
    };

    std::deque<std::string> inv_a_val {
        "pos: 1.0 2.0 3.0",
        "fov: 60", "f: 1000", "a: !!!"
    };

    int NUM_TEST_CASES = 8;
    std::deque<std::string> test_inst[] {
        inv_pos_field, inv_pos_val,
        inv_fov_field, inv_fov_val,
        inv_f_field, inv_f_val,
        inv_a_field, inv_a_val
    };

    bool inst_failed = false;
    for (int i = 0; i < NUM_TEST_CASES; i++)
    {
        inst_failed = false;
        try { c = parse_camera(test_inst[i]); }
        catch (const std::invalid_argument &e){ inst_failed = true; }
        assert (inst_failed);
    }

}


void test_parse_plane()
{
    std::shared_ptr<Plane> p;

    std::deque<std::string> val_plane {
        "nor: 1.0 1.0 1.0", "pos: 2.0 2.0 2.0",
        "amb: 3.0 3.0 3.0", "dif: 4.0 4.0 4.0", "spe: 5.0 5.0 5.0",
        "shi: 6.0"
    };

    p = parse_plane(val_plane);
    assert (p->get_normal(Vec3 { 1.0 }) == Vec3 { 1.0 });
    assert (p->amb == Vec3 { 3.0 });
    assert (p->dif == Vec3 { 4.0 });
    assert (p->spe == Vec3 { 5.0 });
    assert ((p->shi - 6.0) < 0.01);

    std::deque<std::string> inv_normal {
        "nor: 1.0 1.0", "pos: 2.0 2.0 2.0",
        "amb: 3.0 3.0 3.0", "dif: 4.0 4.0 4.0", "spe: 5.0 5.0 5.0",
        "shi 6.0"
    };

    std::deque<std::string> inv_pos {
        "nor: 1.0 1.0 1.0", "pos: a 2.0 2.0",
        "amb: 3.0 3.0 3.0", "dif: 4.0 4.0 4.0", "spe: 5.0 5.0 5.0",
        "shi 6.0"
    };
    
    std::deque<std::string> inv_amb {
        "nor: 1.0 1.0 1.0", "pos: 2.0 2.0 2.0",
        "amb: 3.0 3.0 c", "dif: 4.0 4.0 4.0", "spe: 5.0 5.0 5.0",
        "shi 6.0"
    };

    std::deque<std::string> inv_dif {
        "nor: 1.0 1.0 1.0", "pos: 2.0 2.0 2.0",
        "amb: 3.0 3.0 3.0", "dif: d", "spe: 5.0 5.0 5.0",
        "shi 6.0"
    };

    std::deque<std::string> inv_spe {
        "nor: 1.0 1.0 1.0", "pos: 2.0 2.0 2.0",
        "amb: 3.0 3.0 3.0", "dif: 4.0 4.0 4.0", "specular = five",
        "shi 6.0"
    }; 
    
    std::deque<std::string> inv_shi {
        "nor: 1.0 1.0 1.0", "pos: 2.0 2.0 2.0",
        "amb: 3.0 3.0 3.0", "dif: 4.0 4.0 4.0", "spe: 5.0 5.0 5.0",
        "shi6"
    }; 

    int NUM_TEST_CASES = 6;
    std::deque<std::string> test_inst[] {
        inv_normal, inv_pos,
        inv_amb, inv_dif, inv_spe, inv_shi
    };

    bool inst_failed = false;
    for (int i = 0; i < NUM_TEST_CASES; i++)
    {
        inst_failed = false;
        try { p = parse_plane(test_inst[i]); }
        catch (const std::invalid_argument &e){ inst_failed = true; }
        assert (inst_failed);
    }
}


void test_parse_sphere()
{
    std::shared_ptr<Sphere> s;

    std::deque<std::string> val_sphere {
        "pos: 1.0 1.0 1.0", "rad: 2.0",
        "amb: 3.0 3.0 3.0", "dif: 4.0 4.0 4.0", "spe: 5.0 5.0 5.0",
        "shi: 6.0"
    };

    s = parse_sphere(val_sphere);
    //assert (p->get_normal(Vec3 { 1.0 }) == Vec3 { 1.0 });
    assert (s->amb == Vec3 { 3.0 });
    assert (s->dif == Vec3 { 4.0 });
    assert (s->spe == Vec3 { 5.0 });
    assert ((s->shi - 6.0) < 0.01);

    std::deque<std::string> inv_pos {
        "pos: one", "rad: 2.0",
        "amb: 3.0 3.0 3.0", "dif: 4.0 4.0 4.0", "spe: 5.0 5.0 5.0",
        "shi: 6.0"
    };
    
    std::deque<std::string> inv_r {
        "pos: 1.0 1.0 1.0", "rad: PI",
        "amb: 3.0 3.0 3.0", "dif: 4.0 4.0 4.0", "spe: 5.0 5.0 5.0",
        "shi: 6.0"
    };
    
    std::deque<std::string> inv_amb {
        "pos: 1.0 1.0 1.0", "rad: 2.0",
        "amb: 3.0 3.0", "dif: 4.0 4.0 4.0", "spe: 5.0 5.0 5.0",
        "shi: 6.0"
   };

    std::deque<std::string> inv_dif {
        "pos: 1.0 1.0 1.0", "rad: 2.0",
        "amb: 3.0 3.0 3.0", "dif: 4.0", "spe: 5.0 5.0 5.0",
        "shi: 6.0"
   };

    std::deque<std::string> inv_spe {

        "pos: 1.0 1.0 1.0", "rad: 2.0",
        "amb: 3.0 3.0 3.0", "dif: 4.0 4.0 4.0", "spe/0/0/0",
        "shi: 6.0"
   }; 
    
    std::deque<std::string> inv_shi {
        "pos: 1.0 1.0 1.0", "rad: 2.0",
        "amb: 3.0 3.0 3.0", "dif: 4.0 4.0 4.0", "spe: 5.0 5.0 5.0",
        "shi: six"
    }; 

    int NUM_TEST_CASES = 6;
    std::deque<std::string> test_inst[] {
        inv_r, inv_pos,
        inv_amb, inv_dif, inv_spe, inv_shi
    };
    
    bool inst_failed = false;
    for (int i = 0; i < NUM_TEST_CASES; i++)
    {
        inst_failed = false;
        try { s = parse_sphere(test_inst[i]); }
        catch (const std::invalid_argument &e){ inst_failed = true; }
        assert (inst_failed);
    }
}


void test_parse_light()
{
    std::shared_ptr<Light> l;

    std::deque<std::string> val_light {
        "pos: 1.0 1.0 1.0",
        "amb: 3.0 3.0 3.0", "dif: 4.0 4.0 4.0", "spe: 5.0 5.0 5.0",
    };

    l = parse_light(val_light);
    assert (l->amb == Vec3 { 3.0 });
    assert (l->dif == Vec3 { 4.0 });
    assert (l->spe == Vec3 { 5.0 });

    std::deque<std::string> inv_pos {
        "pos: one", 
        "amb: 3.0 3.0 3.0", "dif: 4.0 4.0 4.0", "spe: 5.0 5.0 5.0",
    };
    
    std::deque<std::string> inv_amb {
        "pos: 1.0 1.0 1.0",
        "amb: 3.0 3.0", "dif: 4.0 4.0 4.0", "spe: 5.0 5.0 5.0",
   };

    std::deque<std::string> inv_dif {
        "pos: 1.0 1.0 1.0",
        "amb: 3.0 3.0 3.0", "dif: 4.0", "spe: 5.0 5.0 5.0",
   };

    std::deque<std::string> inv_spe {
        "pos: 1.0 1.0 1.0",
        "amb: 3.0 3.0 3.0", "dif: 4.0 4.0 4.0", "spe/0/0/0",
   }; 
    
    int NUM_TEST_CASES = 4;
    std::deque<std::string> test_inst[] {
        inv_pos, inv_amb, inv_dif, inv_spe
    };
    
    bool inst_failed = false;
    for (int i = 0; i < NUM_TEST_CASES; i++)
    {
        inst_failed = false;
        try { l = parse_light(test_inst[i]); }
        catch (const std::invalid_argument &e){ inst_failed = true; }
        assert (inst_failed);
    }
}