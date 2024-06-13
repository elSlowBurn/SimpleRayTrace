#pragma once

struct Circle
{
    std::vector<float> center;
    float radius;
    sphere* aSphere;

    Circle(sphere &Sp){
        center.push_back(Sp.center.x / Sp.center.z);
        center.push_back(Sp.center.y / Sp.center.z);
        radius = Sp.radius / Sp.center.z;

        aSphere = &Sp;
    }
};

float countDist(Circle a){
    return sqrt(pow(a.center.at(0), 2) + pow(a.center.at(1), 2));
};
//нужна ли сортировка???
std::vector<Circle> bublesort(std::vector<Circle> vect) { //продумать подходящий алгоритм сортировки!!!
        int k = vect.size();
        //X sort
        for (int i = 0; i + 1 < k; i++){
            for (int j = 0; j + 1 < k - i; j++){
                // if (countDist(vect.at(j + 1)) == countDist(vect.at(j)) && j != 0){
                //     continue; //later
                // }
                if (countDist(vect.at(j + 1)) < countDist(vect.at(j))){
                    std::swap(vect[j], vect[j + 1]);
                }
            }
        }
    return vect;
};

std::vector<float> getMaxMin(std::vector<Circle> a){
    std::vector<float> minMax;
    float max_x = a.at(0).center.at(0) + a.at(0).radius;
    float max_y = a.at(0).center.at(1) + a.at(0).radius;
    float min_x = a.at(0).center.at(0) - a.at(0).radius;
    float min_y = a.at(0).center.at(1) + a.at(0).radius;
    for (int i = 0; i < a.size(); i++){
        if (max_x < a.at(i).center.at(0) + a.at(i).radius) max_x = a.at(i).center.at(0) + a.at(i).radius;
        if (max_y < a.at(i).center.at(1) + a.at(i).radius) max_y = a.at(i).center.at(1) + a.at(i).radius;
        if (min_x > a.at(i).center.at(0) - a.at(i).radius) min_x = a.at(i).center.at(0) - a.at(i).radius;
        if (min_y > a.at(i).center.at(1) - a.at(i).radius) min_y = a.at(i).center.at(1) - a.at(i).radius;
    }
    minMax.push_back(min_x);
    minMax.push_back(min_y);
    minMax.push_back(max_x);
    minMax.push_back(max_y);
    return minMax;
};

struct AABB
{
    float x_coord;
    float y_coord;
    float width;
    float height;
    float borderXMin;
    float borderXMax;
    float borderYMin;
    float borderYMax;
    std::vector<Circle> Circles;

    AABB(float x_coord, float y_coord, float width, float height) 
    {
        x_coord = x_coord;
        y_coord = y_coord;
        width = width;
        height = height;
        borderXMin = x_coord - width / 2;
        borderXMax = x_coord + width / 2;
        borderYMin = y_coord - height / 2;
        borderYMax = y_coord + height / 2;
    }
};


struct Node
{
    AABB* bound;
    Node* child2;
    Node* child1;
    std::vector<Circle> circles;
    int circlesAmmount;

    Node(AABB* bound){
        this->bound = bound;
        this->circlesAmmount = 0;
    }
};

struct BVH
{
    Node* root;

    BVH(Node* root){
        root = root;
        root->child1 = nullptr;
        root->child2 = nullptr;
    }

    void createBVH(std::vector<Circle> circles){
        // std::cout << "Circles:" << std::endl;
        // for (int i = 0; i < circles.size(); i++) std::cout << circles.at(i).aSphere->radius << std::endl;
        // std::vector<Circle> sortedCircles = bublesort(circles); //сортировка кругов
        std::vector<float> boundaries = getMaxMin(circles); //нахождение границ, минимальные максимальные значения
        float x = (boundaries.at(0) + boundaries.at(2)) / 2;
        float y = (boundaries.at(1) + boundaries.at(3)) / 2;
        float width = boundaries.at(2) - boundaries.at(0);
        float height = boundaries.at(3) - boundaries.at(1);
        root = new Node(new AABB(x, y, width, height)); //первый узел с центром в x, y
        root->circlesAmmount = circles.size(); // все круги тут
        divide(circles, this->root); // создание ветвей
        overleapCheck(root->child1, root->child2); // пересечения\перекрытия
    }

    void divide(std::vector<Circle> cir, Node* node){
        if (cir.size() != 1 && cir.size() != 0){
            node->circlesAmmount = cir.size();
            std::vector<Circle> half1 = divideVectorFirstHalf(cir); //деление круггов на первую и вторую половины
            // std::cout << "half1:" << std::endl;
            // for (int i = 0; i < half1.size(); i++)
            // {
            //     std::cout << half1.at(i).aSphere->radius << std::endl;
            // }
            
            std::vector<Circle> half2 = divideVectorSecoondHalf(cir);
            // std::cout << "half2:" << std::endl;
            // for (int i = 0; i < half2.size(); i++)
            // {
            //     std::cout << half2.at(i).aSphere->radius << std::endl;
            // }

            std::vector<float> boundaries = getMaxMin(half1); //определение границ для первой ветви
            float x = (boundaries.at(0) + boundaries.at(2)) / 2;
            float y = (boundaries.at(1) + boundaries.at(3)) / 2;
            float width = boundaries.at(2) - boundaries.at(0);
            float height = boundaries.at(3) - boundaries.at(1);
            node->child1 = new Node(new AABB(x, y, width, height)); //создание первой ветви
            divide(half1, node->child1);

            boundaries = getMaxMin(half2); //опредеоение границ для второй ветви
            x = (boundaries.at(0) + boundaries.at(2)) / 2;
            y = (boundaries.at(1) + boundaries.at(3)) / 2;
            width = boundaries.at(2) - boundaries.at(0);
            height = boundaries.at(3) - boundaries.at(1);
            node->child2 = new Node(new AABB(x, y, width, height)); //создание второй ветви
            divide(half2, node->child2); //рекурсия пока кругов не останется в ветки 1 или 0
        }
        else if (cir.size() == 1){
            node->circles.push_back(cir.at(0));
            node->circlesAmmount = 1;
        }
        else node->circlesAmmount = 0;
    }

    void closestSphere(sphere* A, sphere* B)
    {
        if (A->center.z < B->center.z) 
        {
            A->overleaped = false;
            B->overleaped = true;
        }
        if (A->center.z > B->center.z) 
        {
            A->overleaped = true;
            B->overleaped = false;
        }
        if (A->center.z == B->center.z) {
            if (A->radius > B->radius) 
            {
                A->overleaped = false;
                B->overleaped = true;
            }
            else 
            {
                A->overleaped = true;
                B->overleaped = false;
            }
        }
        // length(A->center)
    }
   
    //поиск пересечений
    bool overleap(AABB* A, AABB* B){
        if ((A->borderXMin >= B->borderXMax ||
            A->borderXMax <= B->borderXMin) ||
            (A->borderYMin <= B->borderYMax ||
            A->borderYMax >= B->borderYMin))
            return false;
        return true;
    }
    
    void overleapCheck(Node* nodeA, Node* nodeB){
        if(overleap(nodeA->bound, nodeB->bound)){
            if (nodeA->circlesAmmount == 1 && nodeB->circlesAmmount == 1){ // листья
                closestSphere(nodeA->circles.at(0).aSphere, nodeB->circles.at(0).aSphere);
                // changeVelocity(nodeA->getCircles().at(0), nodeB->getCircles().at(0));
                // cout << "The circles "<< nodeA->getCircles().at(0)->getId() << " and " << nodeB->getCircles().at(0)->getId() << " collided!" << endl;
            }
            else if (nodeA->circlesAmmount > 1 && nodeB->circlesAmmount > 1){ //рекурсия ветвей
                overleapCheck(nodeA->child1, nodeB->child2);
                overleapCheck(nodeA->child1, nodeB->child1);
                overleapCheck(nodeA->child1, nodeB->child1);
                overleapCheck(nodeA->child1, nodeB->child1);
                overleapCheck(nodeA->child1, nodeA->child1);
                overleapCheck(nodeB->child1, nodeB->child1);
            }
            else if(nodeA->circlesAmmount > 1 && nodeB->circlesAmmount == 1){
                overleapCheck(nodeA->child1, nodeB);
                overleapCheck(nodeA->child1, nodeB);
                overleapCheck(nodeA->child1, nodeA->child1);
            }
            else if(nodeA->circlesAmmount == 1 && nodeB->circlesAmmount > 1){
                overleapCheck(nodeA, nodeB->child1);
                overleapCheck(nodeA, nodeB->child1);
                overleapCheck(nodeB->child1, nodeB->child1);
            }
        }
    }

    std::vector<Circle> divideVectorFirstHalf(std::vector<Circle> original){
        int k = original.size();
        std::vector<Circle> temp;
            for (int i = 0; i < k / 2; i ++){
                temp.push_back(original.at(i));
            }
        return temp;
    }

    std::vector<Circle> divideVectorSecoondHalf(std::vector<Circle> original){
        int k = original.size();
        std::vector<Circle> temp;

        for (int i = k / 2; i < k; i ++){
            temp.push_back(original.at(i));
        }
        return temp;
    }

    void BVH_info(Node* A){
        if(A->circlesAmmount == 1){
            // std::cout << "ammount of circles: " << A->circlesAmmount << std::endl;
            std::cout << "X max = " << A->bound->borderXMax << std::endl;
            std::cout << "X min = " << A->bound->borderXMin << std::endl;
            std::cout << "Y max = " << A->bound->borderYMax << std::endl;
            std::cout << "Y min = " << A->bound->borderYMin << "\n" << std::endl;
        }

        if (A->circlesAmmount > 1){
            BVH_info(A->child1);
            BVH_info(A->child2);
        }
        // else if (A->circlesAmmount == 1)
        // {
        //     // std::cout << A->circles.at(0).aSphere->center.x << std::endl;
        //     std::cout << "X max = " << A->bound->borderXMax << std::endl;
        //     std::cout << "X min = " << A->bound->borderXMin << std::endl;
        //     std::cout << "Y max = " << A->bound->borderYMax << std::endl;
        //     std::cout << "Y min = " << A->bound->borderYMin << std::endl;
            
        // }
        
    }

    bool inTheBoundaries(float x, float y, AABB* box){
        if (x <= box->borderXMax && 
            x >= box->borderXMin &&
            y <= box->borderYMax &&
            y >= box->borderYMin) 
            return true;
        return false;
    }

    sphere* getSphere(float x, float y, Node* A)
    {
        sphere* tmpSphere = nullptr;
        if (A->circlesAmmount == 1 && inTheBoundaries(x, y, A->bound) && A->circles.at(0).aSphere->overleaped == false) 
        {
            tmpSphere = A->circles.at(0).aSphere;
            return tmpSphere;
        }
        else if(A->circlesAmmount > 1 && inTheBoundaries(x, y, A->bound))
        {
            if (getSphere(x, y, A->child1) != nullptr && tmpSphere == nullptr) tmpSphere = getSphere(x, y, A->child1);
            if (getSphere(x, y, A->child2) != nullptr && tmpSphere == nullptr) tmpSphere = getSphere(x, y, A->child2);
        }
        return tmpSphere;
    }
};