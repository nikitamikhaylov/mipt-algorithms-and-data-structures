#include <assert.h>
#include <sstream>
#include <string>
#include <iostream>
#include <limits>
#include <vector>
#include <set>
#include <utility>


class CPosition {
public:
    explicit CPosition( const std::string& source );

    void setAt( int place, unsigned char value );
    unsigned char getAt( int place ) const;

    //Получение эвристики.
    size_t GetHeuristic() const;

    //Последовательность действий, приводящая к данной конфигурации.
    std::string howToGet;

    //Длина пути от исходной до данной конфигурации.
    size_t len() const;

    int nullPlace;

    // Передвижение пустышки.
    CPosition Up() const;
    CPosition Down() const;
    CPosition Right() const;
    CPosition Left() const;

    bool operator== (const CPosition&) const;
    bool operator< (const CPosition&) const;
private:
    unsigned long long data; // 16 ячеек по 4 бита каждая.


    size_t ManhattanDistance() const;
    size_t LinearConflict() const;


};

bool CPosition::operator== (const CPosition& current) const
{
    return this->data == current.data;
}

bool CPosition::operator< (const CPosition& current) const
{
    return this->data < current.data;
}

const unsigned long long Masks[] = {
        0x000000000000000F,
        0x00000000000000F0,
        0x0000000000000F00,
        0x000000000000F000,
        0x00000000000F0000,
        0x0000000000F00000,
        0x000000000F000000,
        0x00000000F0000000,
        0x0000000F00000000,
        0x000000F000000000,
        0x00000F0000000000,
        0x0000F00000000000,
        0x000F000000000000,
        0x00F0000000000000,
        0x0F00000000000000,
        0xF000000000000000,
};

const unsigned long long AntiMasks[] = {
        0xFFFFFFFFFFFFFFF0,
        0xFFFFFFFFFFFFFF0F,
        0xFFFFFFFFFFFFF0FF,
        0xFFFFFFFFFFFF0FFF,
        0xFFFFFFFFFFF0FFFF,
        0xFFFFFFFFFF0FFFFF,
        0xFFFFFFFFF0FFFFFF,
        0xFFFFFFFF0FFFFFFF,
        0xFFFFFFF0FFFFFFFF,
        0xFFFFFF0FFFFFFFFF,
        0xFFFFF0FFFFFFFFFF,
        0xFFFF0FFFFFFFFFFF,
        0xFFF0FFFFFFFFFFFF,
        0xFF0FFFFFFFFFFFFF,
        0xF0FFFFFFFFFFFFFF,
        0x0FFFFFFFFFFFFFFF
};

CPosition::CPosition( const std::string& source ) : data( 0 ), nullPlace( 0 ), howToGet("")
{
    std::istringstream stringStream( source );
    for( char i = 0; i < 16; ++i ) {
        unsigned short value = 0;
        stringStream >> value;
        setAt( i, static_cast<unsigned char>( value ) );
        if( value == 0 ) {
            nullPlace = i;
        }
    }
}

// Установка значения в некоторую позицию.
void CPosition::setAt( int place, unsigned char value )
{
    data = ( data & AntiMasks[place] ) | ( static_cast<long long>( value ) << ( place << 2 ) );
    //std::cout << data << std::endl;
}

// Получение того, что лежит в некоторой позиции.
unsigned char CPosition::getAt( int place ) const
{
    return static_cast<unsigned char>( ( data & Masks[place] ) >> ( place << 2 ) );
}

CPosition CPosition::Up() const
{
    CPosition result( *this );
    // Ставим то, что было выше, на то место, где была пустышка.
    result.setAt( nullPlace, getAt( nullPlace - 4 ) );
    // Ставим пустышку выше.
    result.setAt( nullPlace - 4, 0 );
    result.nullPlace -= 4;
    result.howToGet = "";
    return result;
}

CPosition CPosition::Down() const
{
    CPosition result( *this );
    // Ставим то, что было ниже, на то место, где была пустышка.
    result.setAt( nullPlace, getAt( nullPlace + 4 ) );
    // Ставим пустышку ниже.
    result.setAt( nullPlace + 4, 0 );
    result.nullPlace += 4;
    result.howToGet = "";
    return result;
}

CPosition CPosition::Right() const
{
    CPosition result( *this );
    // Ставим то, что было правее, на то место, где была пустышка.
    result.setAt( nullPlace, getAt( nullPlace + 1) );
    // Ставим пустышку правее.
    result.setAt( nullPlace + 1, 0 );
    result.nullPlace += 1;
    result.howToGet = "";
    return result;
}

CPosition CPosition::Left() const
{
    CPosition result( *this );
    // Ставим то, что было левее, на то место, где была пустышка.
    result.setAt( nullPlace, getAt( nullPlace - 1 ) );
    // Ставим пустышку левее.
    result.setAt( nullPlace - 1, 0 );
    result.nullPlace -= 1;
    result.howToGet = "";
    return result;
}

size_t CPosition::GetHeuristic() const
{
    return ManhattanDistance() + LinearConflict();
}

size_t CPosition::ManhattanDistance() const
{
    size_t answer = 0;
    for(char i = 0; i < 16; ++i) {
        int y = i >> 2;
        int x = i & 3;
        int value = static_cast<int>(getAt(i));
        if (value) {
            value--;
            answer += abs(y - (value >> 2)) + abs (x - (value & 3));
/*
            if (y - (value >> 2) != 0 || x - (value & 3) != 0) {
                ++answer;
            }
*/
        }
    }
    return answer;
}

size_t CPosition::LinearConflict() const
{
    size_t answer = 0;
    for (char i = 0; i < 4; ++i) {
        for (char j = 0; j < 4; ++j) {
            int value = static_cast<int>(getAt(i * 4 + j));
            if (value >= i * 4 + 1 && value <= i * 4 + 4) {
                for (char x = 0; x < j; ++x) {
                    int prev = static_cast<int>(getAt(i * 4 + x));
                    if (prev >= i * 4 + 1 && prev <= i * 4 + 4 && prev > value) {
                        answer += 2;
                    }
                }
            }
        }
    }
    return answer;
}

size_t CPosition::len() const
{
    if (howToGet.size() == 0) {
        return std::numeric_limits<size_t>::max() / 2;
    } else {
        if (howToGet == " ") {
            return 0;
        } else {
            return howToGet.size();
        }
    }
}

CPosition target("1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 0");
size_t MAX_SIZE_T = std::numeric_limits<size_t>::max() / 2;

//true, если нашли. тогда в паре в строке хранится путь
//false, если не нашли, тогда в числе - минимальный f

size_t minimalF;
std::string answer = "";

bool dfs(CPosition current, size_t g, size_t bound) {
    size_t h = current.GetHeuristic();
    if (h == 0) {
        answer = current.howToGet;
        return true;
    }
    size_t f = g + h;
    if (f > bound) {
        if (f < minimalF) {
            minimalF = f;
        }
        return false;
    }
    char lastMove = current.howToGet[current.howToGet.size() - 1];
    int nullPlace = current.nullPlace;
    if (nullPlace >= 4 && lastMove != 'U') {
        CPosition next = current.Up();
        next.howToGet = current.howToGet + 'D';
        if (dfs(next, g + 1, bound)) {
            return true;
        }
    }
    if (nullPlace <= 11 && lastMove != 'D') {
        CPosition next = current.Down();
        next.howToGet = current.howToGet + 'U';
        if (dfs(next, g + 1, bound)) {
            return true;
        }
    }
    if (nullPlace % 4 != 3 && lastMove != 'R') {
        CPosition next = current.Right();
        next.howToGet = current.howToGet + 'L';
        if (dfs(next, g + 1, bound)) {
            return true;
        }
    }
    if ( nullPlace % 4 != 0 && lastMove != 'L'){
        CPosition next = current.Left();
        next.howToGet = current.howToGet + 'R';
        if (dfs(next, g + 1, bound)) {
            return true;
        }
    }
    return false;
}

void Astar(CPosition start) {
    size_t bound = start.GetHeuristic();
    start.howToGet = ' ';
    while (true) {
        minimalF = MAX_SIZE_T;
        if (dfs(start, 0, bound)) {
            break;
        }
        bound = minimalF;
    }
}

bool isReal(CPosition a) {
    int inv = 0;
    for (int i = 0; i < 16; ++i)
        if (a.getAt(i))
            for (int j = 0; j < i; ++j)
                if (a.getAt(j) > a.getAt(i))
                    ++inv;
    for (int i = 0; i < 16; ++i)
        if (a.getAt(i) == 0)
            inv += 1 + i / 4;

    return (inv % 2 == 0);
}

int main() {
    freopen("input.txt", "r", stdin);
    freopen("output.txt", "w", stdout);
    std::string table;
    getline(std::cin, table);
    CPosition start(table);

    if (isReal(start)) {
        Astar(start);
        std::cout << answer.size() - 1 << std::endl;
        std::cout << answer.substr(1, answer.size() - 1) << std::endl;
    }
    else {
        std::cout << "-1" << std::endl;
    }
    return 0;
}
