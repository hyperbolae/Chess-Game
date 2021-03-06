#ifndef lang_hyperbola
#define lang_hyperbola

#include <algorithm>
#include <cstdint>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#define O 1
#define X 2
#define NONE 0
#define SAFETY_HEURISTIC -1.0
#define MIN_HEURISTIC -2.0
#define MAX_HEURISTIC 2.0
typedef uint64_t int64;

static inline int col(int index) { return index & 0x07; }
static inline int row(int index) { return index >> 4; }
static inline int to_index(int row, int col) { return (row << 4) | col; }
static inline int rev(int index) { return index ^ 0x77; }
static inline int ref(int index) { return index ^ 0x07; }
static inline int is_valid_index(int index) { return (index | 0x77) == 0x77; }
static inline int left(int index) { return index - 0x01; }
static inline int right(int index) { return index + 0x01; }
static inline int top(int index) { return index - 0x10; }
static inline int bottom(int index) { return index + 0x10; }
static inline void sort_vec(std::vector<int> &v) { sort(v.begin(), v.end()); }
static inline int ox(int pos, const std::vector<int> &o, const std::vector<int> &x) {
    if (std::find(o.begin(), o.end(), pos) != o.end()) return O;
    if (std::find(x.begin(), x.end(), pos) != x.end()) return X;
    return NONE;
}
static inline void rev_vec(std::vector<int> &v) {
    for (int &index : v) index = rev(index);
}
static inline void ref_vec(std::vector<int> &v) {
    for (int &index : v) index = ref(index);
}

class State {
   private:
    std::vector<int> o;
    std::vector<int> x;
    int64 token_former = 0, token_latter = 0;
    void build_token();
    void require_reflected_and_ordered(bool, bool);

   public:
    State();
    State(const std::vector<int> &, const std::vector<int> &, bool, bool);
    State(const int board[8][8], int);
    State opposite() const;
    int piece_at(int pos) const { return ox(pos, o, x); }
    const std::vector<int> &o_pieces() const { return o; }
    const std::vector<int> &x_pieces() const { return x; }
    size_t hash() const { return token_former ^ token_latter; }
    bool operator==(const State &other) const { return token_former == other.token_former && token_latter == other.token_latter; }
    static bool is_reflected(const std::vector<int> &, const std::vector<int> &);
};

namespace std {
template <>
struct hash<State> {
    long operator()(const State &s) const { return hash<int>()(s.hash()); }
};
}  // namespace std

typedef std::unordered_map<State, std::vector<int>> Expansion;
typedef std::unordered_map<State, Expansion> Record;
const Expansion &expand_state(const State &, Record &);

#endif