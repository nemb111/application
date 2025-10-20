use std::cmp;
use std::collections::HashSet;

#[derive(PartialEq, Eq, Clone, Debug)]
pub enum Bucket {
    One,
    Two,
}

#[derive(PartialEq, Eq, Hash, Clone, Debug)]
struct State(u8, u8);

/// A struct to hold your results in.
#[derive(PartialEq, Eq, Debug)]
pub struct BucketStats {
    /// The total number of "moves" it should take to reach the desired number of liters, including
    /// the first fill.
    pub moves: u8,
    /// Which bucket should end up with the desired number of liters? (Either "one" or "two")
    pub goal_bucket: Bucket,
    /// How many liters are left in the other bucket?
    pub other_bucket: u8,
}

/// Solve the bucket problem
pub fn solve(
    capacity_1: u8,
    capacity_2: u8,
    goal: u8,
    start_bucket: &Bucket,
) -> Option<BucketStats> {
    let mut bucket_solver = BucketSolver::new(capacity_1, capacity_2, goal);

    let state = match start_bucket {
        Bucket::One => {
            bucket_solver.nodes_visited.insert(State(0, capacity_2));
            State(capacity_1, 0)
        }
        Bucket::Two => {
            bucket_solver.nodes_visited.insert(State(capacity_1, 0));
            State(0, capacity_2)
        }
    };

    bucket_solver.solve(vec![state], 1)
}

struct BucketSolver {
    capacity_1: u8,
    capacity_2: u8,
    goal: u8,
    nodes_visited: HashSet<State>,
}

impl BucketSolver {
    fn new(capacity_1: u8, capacity_2: u8, goal: u8) -> Self {
        BucketSolver {
            capacity_1,
            capacity_2,
            goal,
            nodes_visited: HashSet::new(),
        }
    }

    fn solve(&mut self, states: Vec<State>, moves: u8) -> Option<BucketStats> {
        let mut nodes_to_visit = Vec::new();

        for state in states {
            if state.0 == self.goal {
                return Some(BucketStats {
                    moves,
                    goal_bucket: Bucket::One,
                    other_bucket: state.1,
                });
            } else if state.1 == self.goal {
                return Some(BucketStats {
                    moves,
                    goal_bucket: Bucket::Two,
                    other_bucket: state.0,
                });
            }

            let mut succ_states = self.create_succ_states(&state);
            nodes_to_visit.append(&mut succ_states);
            self.nodes_visited.insert(state);
        }

        if nodes_to_visit.is_empty() {
            None
        } else {
            self.solve(nodes_to_visit, moves + 1)
        }
    }

    fn create_succ_states(&self, state: &State) -> Vec<State> {
        let mut res = Vec::new();

        if self.nodes_visited.contains(&state) {
            return vec![];
        }

        // pouring from one bucket to the other
        let bucket2_diff = cmp::min(self.capacity_2 - state.1, state.0);
        res.push(State(state.0 - bucket2_diff, state.1 + bucket2_diff));

        let bucket1_diff = cmp::min(self.capacity_1 - state.0, state.1);
        res.push(State(state.0 + bucket1_diff, state.1 - bucket1_diff));

        // empying one of the buckets
        res.push(State(state.0, 0));
        res.push(State(0, state.1));

        // Filling one of the buckets
        res.push(State(self.capacity_1, state.1));
        res.push(State(state.0, self.capacity_2));

        res
    }
}
