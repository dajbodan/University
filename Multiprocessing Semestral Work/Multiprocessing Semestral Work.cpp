#ifndef __PROGTEST__
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cstdint>
#include <climits>
#include <cfloat>
#include <cassert>
#include <cmath>
#include <iostream>
#include <iomanip>
#include <algorithm>
#include <numeric>
#include <string>
#include <vector>
#include <array>
#include <iterator>
#include <set>
#include <list>
#include <map>
#include <unordered_set>
#include <unordered_map>
#include <queue>
#include <stack>
#include <deque>
#include <memory>
#include <functional>
#include <thread>
#include <mutex>
#include <atomic>
#include <chrono>
#include <stdexcept>
#include <condition_variable>
#include <pthread.h>
#include <semaphore.h>
#include "progtest_solver.h"
#include "sample_tester.h"
using namespace std;
#endif /* __PROGTEST__ */

//-------------------------------------------------------------------------------------------------------------------------------------------------------------
class COptimizer
{
    class MyWrapperPack;
    class MyCompany;
    std :: deque<MyWrapperPack> m_problems;
    std :: mutex m_mutProblems;
    std :: condition_variable m_condEmptyProblems;


    std :: vector<std :: thread> m_workThreads;
    std :: vector<std :: shared_ptr<COptimizer :: MyCompany>> m_company;

    std :: atomic<bool> m_isStopWork;
    struct MyWrapperPack
    {
        MyWrapperPack(void) : m_problemPack(nullptr), m_solved(-1), m_idCompany(-1), m_idPack(-1) {}

        MyWrapperPack(std :: shared_ptr<CProblemPack> problemPack, int idCompany, int idPack) : m_problemPack(problemPack), m_solved(0), m_idCompany(idCompany), m_idPack(idPack) { }

        bool operator < (const COptimizer :: MyWrapperPack & rhs) const { return m_idPack < rhs.m_idPack; }
        bool operator == (const COptimizer :: MyWrapperPack & rhs) const { return m_idPack == rhs.m_idPack; }

        std::shared_ptr<CProblemPack> m_problemPack;
        int m_solved;
        int m_idCompany;
        int m_idPack;
    };

    class MyCompany
    {
    public :
        MyCompany(int id, COptimizer * optimize, ACompany & company)
                :   m_idCompany(id),
                    m_optimize(optimize),

                    m_availableIdPacket(0),
                    m_sendPacket(0)

        {
            m_comapny = company;
            m_isStopCommOutput = false;

        }
        std :: thread & getCommInput(void) { return m_commInput; }
        std :: thread & getCommOutput(void) { return m_commOutput; }
        std :: condition_variable & getCondVariable() { return m_condInputPackets; }

        void sendPacket( std :: unique_ptr<MyWrapperPack> & src)
        {
            std :: lock_guard<std :: mutex> lock(m_mutInputPackets);
            m_inputPackets.push(std :: move(src));
        }

        void activateThreads(void) {
            m_commInput  = std :: thread(&MyCompany :: input, this);
            m_commOutput = std :: thread(& MyCompany :: output, this);
        }
    private  :
        void input(void);
        void output(void);

        void workWithOutput(MyWrapperPack & el);

        int m_idCompany;
        COptimizer * m_optimize;
        ACompany m_comapny;
        std :: thread m_commInput;
        std :: thread m_commOutput;

        std :: queue<shared_ptr<MyWrapperPack>> m_inputPackets;
        std :: set<MyWrapperPack> m_savedPackets;
        std :: mutex m_mutInputPackets;
        std :: condition_variable m_condInputPackets;
        std :: atomic<bool> m_isStopCommOutput;
        int m_availableIdPacket;
        int m_sendPacket;
    };



public:
    COptimizer();
    static bool                        usingProgtestSolver                     ( void )
    {
        return true;
    }
    static void                        checkAlgorithm                          ( AProblem                              problem )
    {
        // dummy implementation if usingProgtestSolver() returns true
    }
    void                               start                                   ( int                                   threadCount );
    void                               stop                                    ( void );
    void                               addCompany                              ( ACompany                              company );

private :

    void work();
    void activateProgtest(std :: pair< std :: shared_ptr<CProgtestSolver>, std :: vector<std :: unique_ptr<COptimizer :: MyWrapperPack>>> & movedProgtest);
    bool insertProgtest(std :: unique_ptr<MyWrapperPack> src,
                        std :: pair< std :: shared_ptr<CProgtestSolver>, std :: vector<std :: unique_ptr<COptimizer :: MyWrapperPack>>> & outProgtest
                        );
    std :: unique_ptr<MyWrapperPack> getElementProblem(void);
    bool isEnd();
    bool isEmptyProgtest(std :: pair< std :: shared_ptr<CProgtestSolver>, std :: vector<std :: unique_ptr<COptimizer :: MyWrapperPack>>> & outProgtest);
    void deliverResult(std :: pair< std :: shared_ptr<CProgtestSolver>, std :: vector<std :: unique_ptr<COptimizer :: MyWrapperPack>>> &);


    void makeProgtestSolverClear();
    // before calling this method, need to mutex progtest;
    std :: pair< std :: shared_ptr<CProgtestSolver>, std :: vector<std :: unique_ptr<COptimizer :: MyWrapperPack>>> substituteProgtest();

    std :: mutex m_mutProgtest;
    std :: pair< std :: shared_ptr<CProgtestSolver>, std :: vector<std :: unique_ptr<COptimizer :: MyWrapperPack>>> m_progtest;
    std :: atomic<int> m_idAvailableCompany;

    int m_cntWorkers = -1;
    std :: mutex m_mutCntWorkers;

};

COptimizer :: COptimizer(void)
{
    m_isStopWork = false;
    m_idAvailableCompany = 0;
    m_progtest = std :: make_pair(createProgtestSolver(), std :: vector<std :: unique_ptr<MyWrapperPack>>());
}
void COptimizer :: start(int threadCount)
{
   for(int i = 0; i < threadCount; i++)
        m_workThreads.emplace_back(std :: thread(& COptimizer :: work, this));

    {
        std :: lock_guard<std :: mutex> lock(m_mutCntWorkers);
        m_cntWorkers= threadCount;
    }


}
void                    COptimizer ::           addCompany                              ( ACompany                              company )
{
    m_company.emplace_back(std :: shared_ptr<MyCompany>(new MyCompany(m_idAvailableCompany, this, company)));
    m_company.back()->activateThreads();
    m_idAvailableCompany++;
}

void COptimizer :: stop(void)
{
    for(long unsigned int i = 0; i < m_company.size(); i++)
        m_company[i]->getCommInput().join();

    {
        std :: lock_guard<std :: mutex> lock(m_mutProblems);
        m_isStopWork = true;
        m_condEmptyProblems.notify_all();
    }

    for(long unsigned int  i = 0; i < m_workThreads.size(); i++)
        m_workThreads[i].join();

    for(long unsigned int i = 0; i < m_company.size(); i++)
    {
        std :: unique_ptr<MyWrapperPack> dummyNullptr = nullptr;
        m_company[i]->sendPacket(dummyNullptr);
        m_company[i]->getCondVariable().notify_one();
        m_company[i]->getCommOutput().join();
    }

}

std :: unique_ptr<COptimizer :: MyWrapperPack> COptimizer ::getElementProblem() {
    std :: unique_lock<std :: mutex> lock(m_mutProblems);
    m_condEmptyProblems.wait(lock, [this]() { return  !m_problems.empty() || m_isStopWork; });

    if(m_problems.empty()) {
        lock.unlock();
        return nullptr;
    }
    auto res = std ::make_unique<MyWrapperPack>(m_problems.front());
    m_problems.pop_front();

    lock.unlock();

    return res;
}

bool COptimizer ::isEnd() {
    std :: lock_guard<std :: mutex> lock(m_mutProblems);
    if(m_isStopWork && m_problems.empty())
        return true;
    return false;

}

std :: pair< std :: shared_ptr<CProgtestSolver>, std :: vector<std :: unique_ptr<COptimizer :: MyWrapperPack>>> COptimizer :: substituteProgtest()
{
    std :: pair< std :: shared_ptr<CProgtestSolver>, std :: vector<std :: unique_ptr<COptimizer :: MyWrapperPack>>> res = std :: move(m_progtest);
    m_progtest = std :: make_pair(createProgtestSolver(), std :: vector<std :: unique_ptr<MyWrapperPack>>());

    if(!m_progtest.first->hasFreeCapacity())
        throw std :: exception();
    return res;
}


bool COptimizer ::insertProgtest
        (std :: unique_ptr<MyWrapperPack> src,
         std :: pair< std :: shared_ptr<CProgtestSolver>, std :: vector<std :: unique_ptr<COptimizer :: MyWrapperPack>>> & outProgtest
        )
{
    std::lock_guard<std::mutex> lock(m_mutProgtest);

    m_progtest.second.push_back(std :: move(src));
    auto & temp = *m_progtest.second.back();

    for (auto it =  std :: next(temp.m_problemPack->m_Problems.begin(), temp.m_solved); it != temp.m_problemPack->m_Problems.end(); ++it) {

        if (m_progtest.first->hasFreeCapacity()) {
            temp.m_solved++;

            m_progtest.first->addProblem(*it);

        }
        else
        {
            outProgtest = substituteProgtest();
            return true;
        }
    }
    if(!m_progtest.first->hasFreeCapacity())
    {
        outProgtest = substituteProgtest();
        return true;
    }
    return false;
}

void COptimizer ::deliverResult(std::pair<std::shared_ptr<CProgtestSolver>, std::vector<std::unique_ptr<COptimizer::MyWrapperPack>>> & src) {




    std::lock_guard<std::mutex> lock2(m_mutProblems);
    int cntPushes = 0;
    for(auto & it : src.second)
    {
        if((long unsigned int)it->m_solved == it->m_problemPack->m_Problems.size()) {

            std :: unique_ptr<MyWrapperPack> && temp = std ::make_unique<MyWrapperPack>(*it);
            m_company[it->m_idCompany]->sendPacket(temp);
            m_company[it->m_idCompany]->getCondVariable().notify_one();

        }
        else
        {

            m_problems.push_front(*it);
            cntPushes++;
        }
    }
    for(int i = 0; i < cntPushes; i++)
        m_condEmptyProblems.notify_one();

}

void COptimizer :: activateProgtest(std :: pair< std :: shared_ptr<CProgtestSolver>, std :: vector<std :: unique_ptr<COptimizer :: MyWrapperPack>>> & movedProgtest)
{
    movedProgtest.first->solve();
    deliverResult(movedProgtest);
}

void COptimizer :: work(void)
{
    while(1)
    {
        std :: pair< std :: shared_ptr<CProgtestSolver>, std :: vector<std :: unique_ptr<COptimizer :: MyWrapperPack>>> movedProgtest;
        movedProgtest.first = nullptr;
        if(isEnd())
        {
            {
                std :: lock_guard<std :: mutex> lock(m_mutCntWorkers);
                if(m_cntWorkers == 1)
                {
                    std :: lock_guard<std :: mutex> lock1(m_mutProgtest);
                    if(!m_progtest.second.empty())
                    {
                        activateProgtest(m_progtest);
                    }
                }
                m_cntWorkers--;
            }
            return;
        }
        auto getElement = getElementProblem();
        bool isFull = false;

        if(getElement != nullptr)
        {

            isFull = insertProgtest(std::move(getElement), movedProgtest);
        }

        if(isFull) {

            activateProgtest(movedProgtest);
        }
    }
}




void COptimizer :: MyCompany :: input(void)
{

    while(1)
    {

        std :: shared_ptr<CProblemPack> element = m_comapny->waitForPack();

        if(element == nullptr)
            return;

        MyWrapperPack wrappedElement( element, m_idCompany, m_availableIdPacket);

        m_availableIdPacket++;
        {
            std :: lock_guard<std :: mutex> lock(m_optimize->m_mutProblems);
            m_optimize->m_problems.push_back(wrappedElement);
            m_optimize->m_condEmptyProblems.notify_one();

        }
    }
}

void COptimizer :: MyCompany ::output(void)
{

    while(1)
    {

        std :: unique_lock<std :: mutex> lock(m_mutInputPackets);

        m_condInputPackets.wait(lock, [this](void) { return ! m_inputPackets.empty();});
        auto res = m_inputPackets.front();
        m_inputPackets.pop();
        if(res == nullptr)
        {

            lock.unlock();
            break;
        }


        lock.unlock();

        workWithOutput(*res);
    }

}
void COptimizer :: MyCompany ::workWithOutput(MyWrapperPack & el)
{

    if(el.m_idPack == m_sendPacket)
    {
        m_sendPacket++;
        m_comapny->solvedPack(el.m_problemPack);

        while(! m_savedPackets.empty() && (*m_savedPackets.begin()).m_idPack == m_sendPacket)
        {

            m_sendPacket++;
            auto first = m_savedPackets.begin();
            m_comapny->solvedPack((*first).m_problemPack);
            m_savedPackets.erase(first);
        }
    }
    else
    {
        m_savedPackets.insert(el);
    }

}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------

#ifndef __PROGTEST__

int                                    main                                    ( void )
{
    COptimizer optimizer;
    ACompanyTest  company = std::make_shared<CCompanyTest> ();
     ACompanyTest  company2 = std::make_shared<CCompanyTest> ();
    // ACompanyTest  company3 = std::make_shared<CCompanyTest> ();
    // ACompanyTest  company4 = std::make_shared<CCompanyTest> ();
    /* ACompanyTest  company5 = std::make_shared<CCompanyTest> ();
     ACompanyTest  company6 = std::make_shared<CCompanyTest> (); */
    optimizer . addCompany ( company );
     optimizer . addCompany ( company2 );
     //optimizer . addCompany ( company3 );
    // optimizer . addCompany ( company4 );
    // optimizer . addCompany ( company5 );
    // optimizer . addCompany ( company6 );


    //optimizer.addCompany(company2);
    optimizer . start ( 10000 );
    //for(int i = 0; i < 1000000000000000; i++) for(int k = 0; k < 1000000000; k++) int j = 1 + 1;
    optimizer . stop  ();
    //printf(company->)
    if ( ! company -> allProcessed () )
        throw std::logic_error ( "(some) problems were not correctly processsed" );
    //   printf("end\n");
    return 0;
}
#endif /* __PROGTEST__ */
