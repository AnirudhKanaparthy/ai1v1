#include <bitset>
#include <cassert>
#include <iostream>
#include <print>
#include <unordered_map>
#include <utility>
#include <vector>

enum class T3Element {
    EMPTY,
    CROSS,
    DOT,
};

std::string_view to_string(T3Element elem) {
    switch(elem) {
        case T3Element::EMPTY: return "_";
        case T3Element::CROSS: return "x";
        case T3Element::DOT  : return "o";
        default: std::unreachable();
    }
}

template<size_t N>
void t3_print_mask(std::bitset<N> mask) {
    for (size_t i = 0; i < N; ++i) {
        std::print("{}", static_cast<int>(mask[i]));
    }
    std::print("\n");
}

template<size_t len>
std::bitset<len*len> t3_horizontal() {
    static bool set{false};
    static std::bitset<len*len> mask = 0;
    if(!set) {
        for(size_t i = 0; i < len; ++i) {mask.set(i);}
        set = true;
    }
    return mask;
}

template<size_t len>
std::bitset<len*len> t3_vertical() {
    static bool set{false};
    static std::bitset<len*len> mask = 0;
    if(!set) {
        for(size_t i = 0; i < len; ++i) {mask.set(len*i);}
        set = true;
    }
    return mask;
}

template<size_t len>
std::bitset<len*len> t3_ldiagonal() {
    static bool set{false};
    static std::bitset<len*len> mask = 0;
    if(!set) {
        for(size_t i = 0; i < len; ++i) {mask.set(len*i+i);}
        set = true;
    }
    return mask;
}
template<size_t len>
std::bitset<len*len> t3_rdiagonal() {
    static bool set{false};
    static std::bitset<len*len> mask = 0;
    if(!set) {
        for(size_t i = 0; i < len; ++i) {mask.set(len*i+(len-i-1));}
        set = true;
    }
    return mask;
}

template <size_t len>
struct T3Board {
    std::bitset<len*len> cross_grid;
    std::bitset<len*len> dot_grid;
    T3Element turn;

    const T3Element operator[](size_t idx) const {
        return get(idx);
    }
    
    const T3Element get(size_t idx) const {
        if(cross_grid[idx]) return T3Element::CROSS;
        if(dot_grid[idx]) return T3Element::DOT;
        return T3Element::EMPTY;
    }

    const T3Element get(size_t x, size_t y) const {
        return get(index_from_pos(x, y));
    }

    void set(size_t idx, T3Element val) {
        switch(val) {
            case T3Element::CROSS: cross_grid.set(idx); break;
            case T3Element::DOT: dot_grid.set(idx); break;
            case T3Element::EMPTY: {
                cross_grid.reset(idx);
                dot_grid.reset(idx);
           } break;
           default: std::unreachable();
        }
    }

    std::string hash_str() const {
        return cross_grid.to_string() + dot_grid.to_string();
    }

    size_t length() const { return len; }
    size_t index_from_pos(size_t x, size_t y) const { return y*len + x; }
    bool is_valid_pos(ssize_t x, ssize_t y) const { return (0 <= x && x < static_cast<ssize_t>(len)) && (0 <= y && y < static_cast<ssize_t>(len)); }

    size_t number_empty() const {
        return len*len - (cross_grid.count() + dot_grid.count());
    }

    static T3Board<len> create() {
        T3Board<len> b{};
        b.cross_grid.reset();
        b.dot_grid.reset();
        b.turn = T3Element::CROSS;
        return b;
    }

    std::optional<T3Element> who_won() const {
        auto n_cross = cross_grid.count();
        auto n_dot = dot_grid.count();
        if(n_cross < n_dot) return std::nullopt;
        if(n_cross < static_cast<ssize_t>(len) && n_dot < static_cast<ssize_t>(len)) return std::nullopt;
        
        const auto total_elems{n_cross + n_dot};

        // Horizontals
        std::bitset<len*len> mask{t3_horizontal<len>()};
        for(size_t i = 0; i < len; ++i) {
            if((cross_grid & mask) == mask) return T3Element::CROSS;
            if((dot_grid & mask) == mask) return T3Element::DOT;
            mask <<= len;
        }

        // Verticals
        mask = t3_vertical<len>();
        for(size_t i = 0; i < len; ++i) {
            if((cross_grid & mask) == mask) return T3Element::CROSS;
            if((dot_grid & mask) == mask) return T3Element::DOT;
            mask <<= 1;
        }

        // Left Diagonal
        mask = t3_ldiagonal<len>();
        if((cross_grid & mask) == mask) return T3Element::CROSS;
        if((dot_grid & mask) == mask) return T3Element::DOT;
        
        // Right Diagonal
        mask = t3_rdiagonal<len>();
        if((cross_grid & mask) == mask) return T3Element::CROSS;
        if((dot_grid & mask) == mask) return T3Element::DOT;
        
        return total_elems == len*len ? std::optional(T3Element::EMPTY) : std::nullopt;
    }

    std::optional<T3Element> who_won(ssize_t x, ssize_t y) const {
        if(x < 0 || y < 0 || !is_valid_pos(x, y)) return who_won();

        // Horizontals
        std::bitset<len*len> mask{t3_horizontal<len>()};
        mask <<= (len*y);
        if((cross_grid & mask) == mask) return T3Element::CROSS;
        if((dot_grid & mask) == mask) return T3Element::DOT;

        // Verticals
        mask = t3_vertical<len>();
        mask <<= x;
        if((cross_grid & mask) == mask) return T3Element::CROSS;
        if((dot_grid & mask) == mask) return T3Element::DOT;

        // Left Diagonal
        mask = t3_ldiagonal<len>();
        if((cross_grid & mask) == mask) return T3Element::CROSS;
        if((dot_grid & mask) == mask) return T3Element::DOT;
        
        // Right Diagonal
        mask = t3_rdiagonal<len>();
        if((cross_grid & mask) == mask) return T3Element::CROSS;
        if((dot_grid & mask) == mask) return T3Element::DOT;
        
        return number_empty() == 0 ? std::optional(T3Element::EMPTY) : std::nullopt;
    }

    T3Element whos_turn() const {
        assert(turn != T3Element::EMPTY);
        return turn;
    }

    void flip_turn() {
        switch(turn) {
            case T3Element::CROSS: turn = T3Element::DOT;   break;
            case T3Element::DOT: turn   = T3Element::CROSS; break;
            default: break;
        }
    }

    bool play_move(T3Element elem, size_t x, size_t y) {
        if(whos_turn() != elem) return false;
        if(!is_valid_pos(x, y)) return false;
        
        const auto idx{index_from_pos(x, y)};
        if(get(idx) != T3Element::EMPTY) return false;

        set(idx, elem);
        flip_turn();
        return true;
    }

    bool play_move_check_win(T3Element elem, size_t x, size_t y) {
        if(whos_turn() != elem) return false;
        if(!is_valid_pos(x, y)) return false;
        
        const auto idx{index_from_pos(x, y)};
        if(get(idx) != T3Element::EMPTY) return false;
        if(who_won().value_or(T3Element::EMPTY) != T3Element::EMPTY) return false;

        set(idx, elem);
        flip_turn();
        return true;
    }

    bool undo_move(T3Element elem, size_t x, size_t y) {
        if(elem == T3Element::EMPTY) return false;
        if(!is_valid_pos(x, y))
            return false;
        const auto idx{index_from_pos(x, y)};
        if(get(idx) != elem) return false;
        
        set(idx, T3Element::EMPTY);
        flip_turn();
        return true;
    }
};

struct T3Move {
    T3Element elem;
    size_t x;
    size_t y;
};

template<size_t len>
std::vector<T3Move> t3_all_possible_moves_check_win(const T3Board<len>& board) {
    std::vector<T3Move> possible_moves;
    possible_moves.reserve(board.number_empty());
    if(board.who_won().value_or(T3Element::EMPTY) != T3Element::EMPTY) return possible_moves;
    
    auto wt{board.whos_turn()};
    for(size_t y = 0; y < len; ++y) {
        for(size_t x = 0; x < len; ++x) {
            const auto e{board.get(x, y)};
            if(e == T3Element::EMPTY) possible_moves.emplace_back(wt, x, y);
        }
    }
    return possible_moves;
}

template<size_t len>
std::vector<T3Move> t3_all_possible_moves(const T3Board<len>& board) {
    std::vector<T3Move> possible_moves;
    possible_moves.reserve(board.number_empty());
    
    auto wt{board.whos_turn()};
    for(size_t y = 0; y < len; ++y) {
        for(size_t x = 0; x < len; ++x) {
            const auto e{board.get(x, y)};
            if(e == T3Element::EMPTY) possible_moves.emplace_back(wt, x, y);
        }
    }
    return possible_moves;
}

template <size_t len>
void t3_print_to_console(const T3Board<len>& board) {
    for(size_t i = 0; i < len; ++i) {
        for(size_t j = 0; j < len; ++j) {
            size_t idx = i*len + j;
            char c{};
            switch(board[idx]) {
                case T3Element::EMPTY: c = '_'; break;
                case T3Element::CROSS: c = 'x'; break;
                case T3Element::DOT  : c = 'o'; break;
                default: std::unreachable();
            }
            std::print("{} ", c);
        }
        std::print("\n");
    }
}

template<size_t len>
std::pair<T3Move, float> t3_best_move(
    const T3Board<len>& board,
    ssize_t last_x,
    ssize_t last_y,
    float alpha,
    float beta,
    std::unordered_map<std::string, std::pair<T3Move, float>>& transposition
) {
    auto key{board.hash_str()};
    if(auto findkey{transposition.find(key)}; findkey != transposition.end()) return findkey->second;

    const auto w_opt{board.who_won(last_x, last_y)};
    if(w_opt) { // Someone won or it's a draw
        float score{};
        switch (w_opt.value()) {
            case T3Element::CROSS: score = +1.0f;  break;
            case T3Element::DOT  : score = -1.0f;  break;
            case T3Element::EMPTY: score =  0.0f;  break;
            default: std::unreachable();
        }
        auto res{std::make_pair(T3Move{}, score)};
        transposition[key] = res;
        return res; // Draw
    }
    
    T3Board<len> t_board{board};
    const auto whos_turn{t_board.whos_turn()};
    
    const auto possible_moves{t3_all_possible_moves(board)};
    std::pair<T3Move, float> best_move{T3Move{}, whos_turn == T3Element::CROSS ? -99999.0f : 99999.0f};
    if(whos_turn == T3Element::CROSS) {
        for(const auto& m : possible_moves) {
            if(!t_board.play_move(m.elem, m.x, m.y)) continue;
            auto best_sub_move{t3_best_move(t_board, m.x, m.y, alpha, beta, transposition)};
            assert(t_board.undo_move(m.elem, m.x, m.y));
            
            if(best_sub_move.second > best_move.second) {
                best_move.first = m;
                best_move.second = best_sub_move.second;
            }
            alpha = std::max(alpha, best_move.second);
            if(beta <= alpha) break;
        }
    } else if(whos_turn == T3Element::DOT) {
        for(const auto& m : possible_moves) {
            if(!t_board.play_move(m.elem, m.x, m.y)) continue;
            auto best_sub_move{t3_best_move(t_board, m.x, m.y, alpha, beta, transposition)};
            assert(t_board.undo_move(m.elem, m.x, m.y));
            
            if(best_sub_move.second < best_move.second) {
                best_move.first = m;
                best_move.second = best_sub_move.second;
            }
            beta = std::min(beta, best_move.second);
            if(beta <= alpha) break;
        }
    } else {std::unreachable();}

    transposition[key] = best_move;
    return best_move;
}

template <size_t len>
std::optional<T3Move> t3_get_move_from_user(const T3Board<len>& board) {
    std::string user_input;
    if(!std::getline(std::cin >> std::ws, user_input)) return std::nullopt;

    auto delim {user_input.find(',')};
    if(delim == user_input.npos) return std::nullopt;

    size_t x{};
    size_t y{};
    auto data{user_input.c_str()};
    std::from_chars(data, data + delim, x);
    std::from_chars(data + delim + 1, data + user_input.size(), y);
    
    return T3Move{board.whos_turn(), x, y};
}

int main() {
    auto b{T3Board<3>::create()};
    
    std::unordered_map<std::string, std::pair<T3Move, float>> transposition;
    auto who_won{b.who_won()};
    for(;;) {
        t3_print_to_console(b);
        std::print(">> ");
        auto moveOpt{t3_get_move_from_user(b)};
        if(!moveOpt) {
            std::print("Wrong input, retry...\n");
            continue;
        }
        auto move{std::move(moveOpt.value())};
        if(!b.play_move(move.elem, move.x, move.y)) break;
        t3_print_to_console(b);
        who_won = b.who_won();
        if(who_won) break;

        const auto [best_move, best_score]{t3_best_move(b, -1, -1, -99999.0f, +99999.0f, transposition)};
        std::print("{}@{},{}={}\n", to_string(best_move.elem), best_move.x, best_move.y, best_score);
        if(!b.play_move(best_move.elem, best_move.x, best_move.y)) break;
        t3_print_to_console(b);
        who_won = b.who_won();
        if(who_won) break;
        std::print("----------\n");
    }
    std::print("----------\n");
    std::print("Final board:\n");
    t3_print_to_console(b);
    std::print("----------\n");
    
    switch(who_won.value()) {
        case T3Element::CROSS: std::print("You won (⊙_⊙)? You must be God.\n"); break;
        case T3Element::DOT:   std::print("You lost. Don't worry, only God can beat the bot. ദ്ദി ˉ͈̀꒳ˉ͈́ )✧\n"); break;
        case T3Element::EMPTY: std::print("It's a draw. You are good! (੭˃ᴗ˂)੭\n"); break;
        default: std::unreachable();
    }

    return 0;
}
