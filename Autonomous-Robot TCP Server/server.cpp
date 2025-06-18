#include <iostream>
//using namespace std;

#include <cstdlib>
#include <cstdio>
#include <sys/socket.h> // socket(), bind(), connect(), listen()
#include <unistd.h> // close(), read(), write()
#include <netinet/in.h> // struct sockaddr_in
#include <strings.h> // bzero()
#include <unordered_map>
#include <vector>
#include <deque>
#include <sstream>
#include <cstring>
#include <memory>
#define BUFFER_SIZE 10240
#define TIMEOUT 100000 

using coordinates = std :: pair<int, int>;
std :: unordered_map<int, std :: pair<int, int>> map_Authenzation
{
    {0, {23019, 32037}},
    {1, {32037, 29295}},
    {2, {18789, 13603}},
    {3, {16443, 29533}},
    {4, {18189, 21952}}
};

enum class STATES
{
    USERNAME, KEY_ID, CONFIRMATION, UNKNOWN, MOVING
};
    
enum class DIRECTION
{
    UP, DOWN, LEFT, RIGHT, FORWARD, UNKNOWN
};


enum class  DataReceive
{
    CLIENT_OK, CLIENT_MESSAGE , UNKNOWN
};

enum class INITIALIZIATION_READ
{
   UNKOWN, BEGIN, FIRST, SECOND, THIRD, FOURTH, END
};

class SERVER_KEY_OUT_OF_RANGE_ERROR
{

};

class SERVER_SYNTAX_ERROR
{

};

class SERVER_LOGIN_FAILED
{

};

std :: ostream & operator << (std :: ostream & os, DIRECTION & dir)
{
    switch(dir)
    {
        case DIRECTION :: UP :
            return  os << " UP ";

        case DIRECTION :: DOWN :
            return  os << " DOWN ";

        case DIRECTION :: LEFT :
            return os << " LEFT ";
        
        case DIRECTION :: RIGHT :
            return os << " RIGHT ";

        case DIRECTION :: FORWARD :
            return os << " FORWARD ";

        case DIRECTION :: UNKNOWN :
            return os << " UNKNOWN ";
    }
    return os;
}

int sumAscii(const char * src, int length)
{
    int res = 0;
    for(int i = 0; i < length; i++)
        res += src[i];
    return res;
}
bool checkCorretnessMessage(std :: string & src, STATES  state = STATES :: UNKNOWN )
{
    if(state == STATES :: UNKNOWN )
    {
        if(src.size() < 2)
            return false;
        else
            return src.at(src.size() - 2) == '\a' && src.at(src.size() - 1) == '\b';
    }
    return true; 
}

int calcHashToClient(int key, int sumUsername )
{
    return (((sumUsername * 1000) % 65536) + map_Authenzation[key].first) % 65536;
}
int calcHashFromClient(int key, int sumUserName)
{
    int ret =  (((sumUserName * 1000) % 65536) + map_Authenzation[key].second) % 65536;
    std :: cout << ret << '\n';
    return ret;
}

bool checkBufferForEnd(char * buffer, int length)
{
    if(length < 2)
    {
        
        return false;
    }
    return buffer[length - 1] == '\b' && buffer[length - 2] == '\a';
}

void checkConfirmation(std :: string & str)
{
    for(int i = 0; i < str.size(); i++)
    {
        if(! std :: isdigit(str.at(i)))
            throw SERVER_SYNTAX_ERROR();
    }
}

void proccessingUsername(int c, char * buffer, int bytesRead, STATES & state, int & outSurname)
{
    outSurname = sumAscii(buffer, bytesRead);
    std :: cout << "SumUserName : " << outSurname << "\n";
    if(std :: string temp = ("107 KEY REQUEST\a\b"); send(c, temp.c_str(), temp.size(), MSG_NOSIGNAL) < 0)
    {
        std :: cout << "data se neposlali\n";
        close(c);
        throw std :: exception();
    }
    state = STATES :: KEY_ID;
}

void proccessingKeyID(int c, char * buffer, int bytesRead, int & key, int sumUsername, STATES & state)
{
    std :: cout << "Key ID : buffer :  " << buffer << "\n";
    if( ! std :: isdigit(buffer[0]))
        throw SERVER_SYNTAX_ERROR();
    key = atoi(buffer);
    if(key < 0 || key > 4)
        throw SERVER_KEY_OUT_OF_RANGE_ERROR();
    std :: cout << "key : " << key << "\n";
    if(std :: string temp = std :: to_string(calcHashToClient(key, sumUsername)) + "\a\b"; send(c, temp.c_str(), temp.size(), MSG_NOSIGNAL) < 0)
    {
        std :: cout << "data se neposlali\n";
        close(c);
        throw std :: exception();
    }
    state = STATES :: CONFIRMATION;
}

void proccessignConfirmation(int c, char * buffer, int bytesRead, int & key, int & sumUsername, STATES & state)
{
    std :: cout << buffer << "\n";
    std :: string str(buffer);
    checkConfirmation(str);
    if(int hashFromClient = calcHashFromClient(key, sumUsername); hashFromClient != atoi(buffer))
    {
        throw SERVER_LOGIN_FAILED();
        std :: cout << "Hash from Client does not match, FUCK UP\n";
        close(c);
        throw std :: exception();
    }
    if( std :: string sendVal("200 OK\a\b"); send(c, sendVal.c_str(), sendVal.size(), MSG_NOSIGNAL) < 0)
    {
        std :: cout << "Data does not send \n";
        close(c);
        throw std :: exception();
    }
    state = STATES :: MOVING;
}

void copyFromBufferToUniqPtr(char * tempBuffer, int start, int length, std :: unique_ptr<char []> & src)
{
    int j = 0;
    for(int i = start; i < length; ++j, ++i)
    {
        src[j] = tempBuffer[i];
    }
    src[length - 1] = '\0';
}

std :: deque<std :: pair< std :: unique_ptr<char []>, int>> parseInput(char * input, int length)
{
    std :: deque<std :: pair < std :: unique_ptr<char []>, int > > res;
    char tempBuffer[5000];
    for(int i = 1, start = 0; i < length; i++)
    {
        if(input[i - 1] == '\a' && input[ i ] == '\b')
        {
            
            strncpy(tempBuffer, input, i - 1);
            tempBuffer[i - 1] = '\0';
            res.push_back( { std :: unique_ptr<char []>(new char [i]), i}); // i - 1 -> know with exp
            copyFromBufferToUniqPtr(tempBuffer, start, res.back().second, res.back().first);
            start = i + 1;
        }
    }
    return res;
}

bool StartCommunication(int c, STATES & state)
{
    std :: string str;
    const int lengthBuffer = 100;
    char buffer[lengthBuffer];
    int sumUsername = 0;
    int key = 0;
    while(true)
    {
        int bytesRead = recv(c, &buffer, lengthBuffer - 2, 0 );
        buffer[bytesRead] = '\0';
        std :: deque<std :: pair <std :: unique_ptr<char []>, int > > res = parseInput(buffer, bytesRead);
       /* if(bytesRead <= 0)
        {
            close(c);
            return false;
        }
        std :: cout << "BytesRead : "<< bytesRead << "\n";
        if(! checkBufferForEnd(buffer, bytesRead))
        {
            std :: cout << "read : " << bytesRead << "\n";
            //std :: cout << "prevLast : " << (int)(buffer[bytesRead - 2]) << ", Last : " << (int)(buffer[bytesRead - 1]) << "\n"; 
            perror("error with end of recieve message");
            throw std :: exception();
        }*/
        /*if(bytesRead > 2)
        {
            std :: cout << ">" <<  buffer[bytesRead - 3] << "<" << ",>" << buffer[bytesRead - 2] << "<\n";
            buffer[bytesRead - 2] = '\0';
            bytesRead -= 2;
        }
        else 
        {
            buffer[1] = '\0';
            buffer[2] = '\b';
            bytesRead -= 2;
        }
        std :: cout << "str : >" << str << "<\n";*/
        std :: cout << "size : " << res.size() << "\n";
        std :: cout << "First  : >" << res[0].first.get() << "<" << std :: endl;
        std :: cout << "Second : >" << res[1].first.get() << "<" <<std :: endl;
        std :: cout << "Third : " << res[2].first.get() << "<" << std :: endl;
        while(res.size() != 0)
        {
            switch (state)
            {
                case STATES :: USERNAME:
                    proccessingUsername(c, res.front().first.get(), res.front().second,  state, sumUsername);
                    break;
            case STATES :: KEY_ID : 
                    proccessingKeyID(c, res.front().first.get(), res.front().second, key, sumUsername, state);
                    break;
                case STATES :: CONFIRMATION :
                {
                    proccessignConfirmation(c, res.front().first.get(), res.front().second, key, sumUsername, state);
                    return true;
                }
            }
            res.pop_front();
        }
    }
    return true;
}

bool initializeSocket(int & varSocket)
{
    
    varSocket = socket(AF_INET, SOCK_STREAM, 0);
    if(varSocket < 0)
    {
        perror("Nemohu vytvorit socket: ");
        return false;
    }
    return true;
}

bool checkPort(int & port, int & varSocket)
{
    if(port == 0){
        std :: cerr << "Usage: server port" << std :: endl;
        close(varSocket);
        return -1;
    }
    return true;
}

bool initializeSocketAddr(struct sockaddr_in & adresa, int socket, int port)
{
    bzero(&adresa, sizeof(adresa));
    adresa.sin_family = AF_INET;
    adresa.sin_port = htons(port);
    adresa.sin_addr.s_addr = htonl(INADDR_ANY);

    if(bind(socket, (struct sockaddr *) &adresa, sizeof(adresa)) < 0){
        perror("Problem s bind(): ");
        close(socket);
        return false;
    }
    return true;
}

bool makeSocketListening(int & socket)
{
    if(listen(socket, 10) < 0){
        perror("Problem s listen()!");
        close(socket);
        return false;
    }
    return true;
}

bool acceptClient(int & clientSocket, int & serverSocket, struct sockaddr * vzdalenaAddresa, socklen_t & velikost)
{
    clientSocket = accept(serverSocket, (struct sockaddr *) &(*vzdalenaAddresa), &velikost);
    if(clientSocket < 0){
        perror("Problem s accept()!");
        close(serverSocket);
        return false;
    }
    return true;
}


std :: pair<DataReceive, std :: pair<int, int>> recieveData(int clientSocket, DataReceive expected)
{
    char buffer[BUFFER_SIZE];
    int bytesRead = recv(clientSocket, &buffer, BUFFER_SIZE - 1, 0);
    if(bytesRead < 0)
    {
        perror("recv() is fucked up\n");
        throw std :: exception();
    }
    if(bytesRead < 2 || (buffer[bytesRead - 2] != '\a' || buffer[bytesRead - 1] != '\b'))
    {
        perror("recv() got not enough bytes\n");
        throw std :: exception();
    }
    buffer[BUFFER_SIZE - 1] = '\0';
    if(bytesRead > 2)
        {
            std :: cout << ">" <<  buffer[bytesRead - 3] << "<" << ",>" << buffer[bytesRead - 2] << "<\n";
            buffer[bytesRead - 2] = '\0';
        }
        else 
        {
            buffer[1] = '\0';
            buffer[2] = '\b';
        }
    std :: string temp(buffer);

    if(temp.size() > 1 && temp.at(0) == 'O' && temp.at(1) == 'K') // should I add here expected OK ?
    {
        int x = 0;
        int y = 0;
        std :: istringstream iss { temp };
        std :: string strOK;
        std :: pair<DataReceive, coordinates> res { DataReceive :: CLIENT_OK, {-1,-1} };
        iss >> strOK >> res.second.first >> res.second.second;
        return res;
    }
    else if(expected == DataReceive ::CLIENT_MESSAGE)
    {
        return std :: pair<DataReceive, coordinates> {DataReceive :: CLIENT_MESSAGE , {-1, -1}};
    }
}

void sendMove(int clientSocket, std :: string src)
{
    if(send(clientSocket, src.c_str(), src.length(), MSG_NOSIGNAL) < 0)
    {
        perror("send message fucked up\n");
        throw std :: exception();
    }
}

void makeMove(int clientSocket, DIRECTION move)
{
    switch(move)
    {
        case DIRECTION :: LEFT :
        {
            sendMove(clientSocket, std :: string ("103 TURN LEFT\a\b"));
            break;
        }
        case DIRECTION :: RIGHT :
        {
            sendMove(clientSocket, std :: string("104 TURN RIGHT\a\b"));
            break;
        }
        case DIRECTION :: FORWARD :
        {
            sendMove(clientSocket, std :: string("102 MOVE\a\b"));
            break;
        }
    }
}

void checkMessageOK(DataReceive receive)
{
    if(receive != DataReceive :: CLIENT_OK)
    {
        perror("not message OK from client\n");
        throw std :: exception();
    }
}

DIRECTION calcDirection(std :: pair<int, int> & prev, std :: pair<int, int> & next)
{
    std :: cout << "prev : " << "(" << prev.first << "," << prev.second << ") \n";
    std :: cout << "next : " << "(" << next.first << "," << next.second << ") \n";
    if(prev.first == -1 && prev.second == -1) // prev is initialize val
        return DIRECTION :: UNKNOWN;
    if(prev.first == next.first && prev.second == next.second)
        return DIRECTION :: UNKNOWN;
    else if(prev.first == next.first && prev.second - 1 == next.second)
        return DIRECTION :: DOWN;
    else if(prev.first == next.first && prev.second + 1 == next.second)
        return DIRECTION :: UP;
    else if(prev.first - 1 == next.first && prev.second == next.second)
        return DIRECTION :: LEFT;
    else if(prev.first + 1 == next.first && prev.second == next.second)
        return DIRECTION :: RIGHT;
    perror("error Direction\n");
    throw std :: exception();
}


INITIALIZIATION_READ getNextRead(INITIALIZIATION_READ & src, DIRECTION & dir)
{
    INITIALIZIATION_READ res = INITIALIZIATION_READ :: UNKOWN;

    switch(src)
    {
        case INITIALIZIATION_READ :: FIRST :
            res = INITIALIZIATION_READ :: SECOND;
            dir = DIRECTION :: FORWARD;
            break;
        case INITIALIZIATION_READ :: SECOND :
            res = INITIALIZIATION_READ :: THIRD;
            dir = DIRECTION :: RIGHT;
            break;
        case INITIALIZIATION_READ :: THIRD :
            res = INITIALIZIATION_READ :: FOURTH;
            dir = DIRECTION :: FORWARD;
            break;
        case INITIALIZIATION_READ :: FOURTH :
            res = INITIALIZIATION_READ :: END;
            dir = DIRECTION :: UNKNOWN;
            break;
        default :
            perror("INITIALIZIATION_READ next mistake, go and fix it up, bro");
            throw std :: exception();
    }

    return res;
}

std :: pair<coordinates, DIRECTION> initializeRobot(int & clientSocket)
{
    std :: pair<coordinates, DIRECTION> res { {-1,-1}, DIRECTION :: UNKNOWN};
    INITIALIZIATION_READ readIterator = INITIALIZIATION_READ :: FIRST;
    std :: pair<DataReceive, coordinates> recieve{ DataReceive :: UNKNOWN, {-1, -1} };

    DIRECTION dir = DIRECTION ::LEFT;
    DIRECTION dirRobot = DIRECTION :: UNKNOWN;
    std :: pair<DataReceive, coordinates> temp {DataReceive :: UNKNOWN, {-1, -1} };
    for(; readIterator != INITIALIZIATION_READ :: END; readIterator = getNextRead(readIterator, dir))
    {
        std :: cout << "it : val : "<<  dir << "\n";
        makeMove(clientSocket, dir);
         temp = recieveData(clientSocket, DataReceive :: CLIENT_OK);
        checkMessageOK(temp.first);
        DIRECTION retDir =  calcDirection(recieve.second, temp.second);
        if(retDir != DIRECTION :: UNKNOWN)
        {
            res.second = retDir;
            break;
        }
        recieve = std :: make_pair(temp.first, temp.second);
        
        
    }
    res.first = std :: make_pair(temp.second.first, temp.second.second);
    return res;
}

DIRECTION getNextDirection(DIRECTION dir)
{
    switch(dir)
    {
        case DIRECTION :: DOWN :
            return DIRECTION :: LEFT;
        case DIRECTION :: LEFT :
            return DIRECTION :: UP;
        case DIRECTION :: UP :
            return DIRECTION :: RIGHT;
        case DIRECTION :: RIGHT :
            return DIRECTION :: DOWN;
    }
    perror("Unexpected next direction");
    throw std :: exception();
}

void makeRotation(int clientSocket, DIRECTION  src, DIRECTION  dst)
{
    while(src != dst)
    {
        std :: cout << src << "\n";
        makeMove(clientSocket, DIRECTION :: RIGHT);
        std :: pair<DataReceive, coordinates> retDir = recieveData(clientSocket, DataReceive ::CLIENT_OK);
        checkMessageOK(retDir.first);
        src = getNextDirection(src);
    }
}

void moveAccordingCollumns(int & clientSocket,  DIRECTION & dir, coordinates & pos)
{
    if(pos.first == 0)
        return ;
    std :: cout << "Moving according collumn\n";
    if(pos.first < 0)
    {
        if(dir != DIRECTION :: RIGHT)
            makeRotation(clientSocket, dir, DIRECTION :: RIGHT);
        dir =  DIRECTION :: RIGHT;
        while(!(pos.first == 0))
        {
            makeMove(clientSocket, DIRECTION :: FORWARD);
            std :: pair<DataReceive, coordinates> retDir = recieveData(clientSocket, DataReceive :: CLIENT_OK);
            checkMessageOK(retDir.first);
            
            pos.first = retDir.second.first;
            pos.second = retDir.second.second;
            std :: cout << "(" << pos.first << "," << pos.second << ")" << "\n";
        }
    }
    else
    {
        if(dir != DIRECTION :: LEFT)
            makeRotation(clientSocket, dir, DIRECTION :: LEFT);
        dir = DIRECTION :: LEFT;
        while(!(pos.first == 0))
        {
            makeMove(clientSocket, DIRECTION :: FORWARD);
            std :: pair<DataReceive, coordinates> retDir = recieveData(clientSocket, DataReceive :: CLIENT_OK);
            checkMessageOK(retDir.first);
           
            pos.first = retDir.second.first;
            pos.second = retDir.second.second;
            std :: cout << "(" << pos.first << "," << pos.second << ")" << "\n";
        }
    }
}

void moveAccordingRow(int & clientSocket, DIRECTION & dir, coordinates & pos)
{
    std :: cout << "row : " << pos.first << ", " << pos.second << "\n";
    if(pos.second == 0)
        return ;
    if(pos.second < 0)
    {
        if(dir != DIRECTION :: UP)
            makeRotation(clientSocket, dir, DIRECTION :: UP);
        dir = DIRECTION :: UP;
        while(!(pos.second == 0))
        {
            makeMove(clientSocket, DIRECTION :: FORWARD);
            std :: pair<DataReceive, coordinates> retDir = recieveData(clientSocket, DataReceive :: CLIENT_OK);
            checkMessageOK(retDir.first);
            
            pos.first = retDir.second.first;
            pos.second = retDir.second.second;
            std :: cout << "(" << pos.first << "," << pos.second << ")" << "\n";
        }
    }
    else
    {
        if(dir != DIRECTION :: DOWN)
            makeRotation(clientSocket, dir, DIRECTION :: DOWN);
        dir = DIRECTION :: DOWN;
        while(!(pos.second == 0))
        {
            makeMove(clientSocket, DIRECTION :: FORWARD);
            std :: pair<DataReceive, coordinates> retDir = recieveData(clientSocket, DataReceive :: CLIENT_OK);
            checkMessageOK(retDir.first);
            
            pos.first = retDir.second.first;
            pos.second = retDir.second.second;
            std :: cout << "(" << pos.first << "," << pos.second << ")" << "\n";
        }
    }

}

bool moveToCentr(int & clientSocket, STATES & state, coordinates & out)
{
    if(state != STATES ::MOVING)
        return false;
    std :: cout << "Start moving\n";
    std :: pair<coordinates, DIRECTION> startPos = initializeRobot(clientSocket);
    std :: cout << "start pos : " << startPos.first.first << ", " << startPos.first.second <<  " Direction : " << startPos.second << "\n";
    moveAccordingCollumns(clientSocket, startPos.second, startPos.first );
    moveAccordingRow(clientSocket, startPos.second, startPos.first);
    out = std :: make_pair(startPos.first.first, startPos.first.second);
    return true;
}

bool getPickUpMessage(int clientSocket, coordinates cor)
{
    if(cor.first == 0 && cor.second == 0)
    {
        sendMove(clientSocket, "105 GET MESSAGE\a\b");
        std :: pair<DataReceive, coordinates> retVal = recieveData(clientSocket, DataReceive :: CLIENT_MESSAGE);
        if(retVal.first == DataReceive :: CLIENT_MESSAGE)
        {
            sendMove(clientSocket, "106 LOGOUT\a\b");
            return true;
        }


    }
}

int main(int argc, char **argv) 
{

    if(argc < 2){
        std :: cerr << "Usage: server port" << std :: endl;
        return -1;
    }
    int serverSocket = 0;
    int port = atoi(argv[1]);
    struct sockaddr_in addresa;
    if( int retVal = initializeSocket(serverSocket);
     !  (
            retVal &&
            checkPort(port, serverSocket) &&
            initializeSocketAddr(addresa, serverSocket, port) 
        )
     )
        return retVal;
    if(!makeSocketListening(serverSocket))
        return -1;
    struct sockaddr vzdalena_adresa;
    socklen_t velikost;
    int clientSocket = 0;
    while(true)
    {
        if(! acceptClient(clientSocket, serverSocket, &vzdalena_adresa, velikost))
            return -1;

        pid_t pid = fork();
        if(pid == 0){
            close(serverSocket);
            fd_set sockets;
            STATES state = STATES :: USERNAME;
            int retval;
            char buffer[BUFFER_SIZE];
            while(true){
                struct timeval timeout;
                timeout.tv_sec = TIMEOUT;
                timeout.tv_usec = 0;
                FD_ZERO(&sockets);
                FD_SET(clientSocket, &sockets);
                coordinates cor;
                retval = select(clientSocket + 1, &sockets, NULL, NULL, &timeout);
                if(retval < 0){
                    perror("Chyba v select(): ");
                    close(clientSocket);
                    return -1;
                }
                if(!FD_ISSET(clientSocket, &sockets)){
                    std :: cout << "Connection timeout!" << std :: endl;
                    close(clientSocket);
                    return 0;
                }
                try{
                    if(! StartCommunication(clientSocket, state))
                    {
                        return -3;
                    }
                }
                catch(SERVER_SYNTAX_ERROR & ex)
                {
                    std :: cout << "Syntax Error\n";
                    sendMove(clientSocket, "301 SYNTAX ERROR\a\b");
                    close(clientSocket);
                    return -3;
                }
                catch(SERVER_KEY_OUT_OF_RANGE_ERROR & ex)
                {
                    sendMove(clientSocket, "303 KEY OUT OF RANGE\a\b");
                    close(clientSocket);
                    std :: cout << "key out of range errror\n";
                    return -3;
                }
                catch(SERVER_LOGIN_FAILED & ex)
                {
                    sendMove(clientSocket, "300 LOGIN FAILED\a\b");
                    close(clientSocket);
                    return -4;
                }
                catch(...)
                {
                    std :: cout << "FUUUUUCK\n";
                }
                if(! moveToCentr(clientSocket, state, cor))
                {
                    return -4;
                }
                if(getPickUpMessage(clientSocket, cor ))
                {
                    close(clientSocket);
                    return 0;
                }
            }
            close(clientSocket);
            return 0;
        }

        int status = 0;
       // waitpid(0, &status, WNOHANG);
        close(clientSocket);
    }
    
    close(serverSocket);
    return 0;
}
