#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <thread>
#include <chrono>

double times[150];
unsigned int count = 0;
void randToVec(std::vector<int>& vec, const int& rep)
{
    srand(time(NULL));
    for (auto i = 0; i < rep; i++)
        vec.push_back(rand());
}

void sum(std::vector<int>& vec1, std::vector<int>& vec2)
{
    auto begin = std::chrono::steady_clock::now();
    int summ = 0;
    for (auto i = 0; i < vec1.size(); i++)
        vec1[i] += vec2[i];
    auto end = std::chrono::steady_clock::now();
    auto elapsed_ms = std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin);
    times[count++] = elapsed_ms.count() / 10000000.0;
}

void divVec(std::vector<std::vector<int>>& vectors, size_t& threads, std::vector<int>& vec)
{
    unsigned int n = 0;
    std::vector<int> tmp;
    for (auto i = 0; i < threads; i++)
    {
        for (auto j = 0; j < vec.size() / threads; j++)
        {
            tmp.push_back(vec[n]);
            n++;
        }
        vectors.push_back(tmp);
        tmp.clear();
    }
}

int main()
{
    size_t threads = 1;
    unsigned int use_Time = 0;
    int numbers[4] = { 1000, 10000, 100000, 1000000 };
    std::vector<int> vec1, vec2;
    std::vector<std::vector<int>> vectors1, vectors2;
    std::vector<std::thread> thr;
    int numCPU = std::thread::hardware_concurrency();
    std::cout << "Number of hardware cores :" << numCPU << std::endl;
    std::cout << "\t\t" << numbers[0] << "\t" << numbers[1] << "\t" << numbers[2] << "\t" << numbers[3] << std::endl;
    std::cout << "Threads " << threads << "\t";
    for (auto j = 0; j <= 5; j++)
    {

        for (auto i = 0; i < sizeof(numbers) / sizeof(numbers[0]); i++)
        {
            std::thread frs(randToVec, std::ref(vec1), std::ref(numbers[i]));
            std::thread sec(randToVec, std::ref(vec2), std::ref(numbers[i]));
            frs.join();
            sec.join();
            if (threads == 1)
            {
                sum(vec1, vec2);
            }
            if (threads > 1)
            {
                divVec(vectors1, threads, vec1);
                divVec(vectors2, threads, vec2);
                for (auto c = 1; c <= threads; c++)
                {
                    thr.emplace_back(sum, std::ref(vectors1[c - 1]), std::ref(vectors2[c - 1]));
                }
                for (auto& th : thr)
                    th.join();
                vectors1.clear();
                vectors2.clear();
                thr.clear();

            }
            vec1.clear();
            vec2.clear();
            double tmp1 = 0;
            int tmp2 = threads + use_Time;
            for (int m = use_Time; m < tmp2; m++)
            {
                tmp1 += times[m];
                use_Time++;
            }
            std::cout << tmp1 << "\t";
        }
        if (threads == 16)
            break;
        std::cout << std::endl;
        threads = threads * 2;
        std::cout << "Threads " << threads << "\t";
    }
}
