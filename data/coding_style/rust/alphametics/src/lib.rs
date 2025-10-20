use std::collections::HashMap;
use std::collections::HashSet;
use std::collections::VecDeque;
use std::hash::BuildHasherDefault;
use std::iter::FromIterator;

use hashers::fx_hash::FxHasher;

pub fn solve(input: &str) -> Option<HashMap<char, u8>> {
    let mut solver = Solver::new(input);

    if let Some(hm) = solver.solve() {
        Some(hm.into_iter().collect())
    } else {
        None
    }
}

struct Solver<'a> {
    operand_chars: Vec<&'a str>,
    sum_chars: Vec<&'a str>,
    unique_chars: Vec<char>,
    unique_digits: VecDeque<u8>,
    char_number_mapping: HashMap<char, u8, BuildHasherDefault<FxHasher>>,
}

impl<'a> Solver<'a> {
    pub fn new(input: &'a str) -> Self {
        let parsed_input = input
            .split("==")
            .map(|x| x.trim().split('+').map(|x| x.trim()).collect())
            .collect::<Vec<Vec<&str>>>();

        let operand_chars = parsed_input.get(0).unwrap().to_vec();
        let sum_chars = parsed_input.get(1).unwrap().to_vec();
        let unique_chars = operand_chars
            .iter()
            .chain(sum_chars.iter())
            .flat_map(|s| s.chars())
            .collect::<HashSet<_>>()
            .into_iter()
            .collect::<Vec<_>>();
        let unique_digits = VecDeque::from_iter(vec![0, 1, 2, 3, 4, 5, 6, 7, 8, 9]);
        let char_number_mapping = HashMap::with_hasher(BuildHasherDefault::<FxHasher>::default());

        Self {
            operand_chars,
            sum_chars,
            unique_chars,
            unique_digits,
            char_number_mapping,
        }
    }

    fn solve(&mut self) -> Option<HashMap<char, u8, BuildHasherDefault<FxHasher>>> {
        let curr_char = self.unique_chars.pop().unwrap();
        for i in 0..self.unique_digits.len() {
            let num = self.unique_digits.remove(i).unwrap();
            self.char_number_mapping.insert(curr_char, num);

            if self.unique_chars.is_empty() {
                if self.check_solution() {
                    return Some(self.char_number_mapping.clone());
                }
            } else if let Some(res) = self.solve() {
                return Some(res);
            }
            self.unique_digits.insert(i, num);
        }
        self.unique_chars.push(curr_char);
        self.char_number_mapping.remove(&curr_char);

        None
    }

    fn check_solution(&mut self) -> bool {
        let is_first_chars_not_zero = |input: &Vec<&str>| {
            !input
                .iter()
                .map(|s| self.char_number_mapping.get(&s.chars().next().unwrap()))
                .take_while(|x| x.is_some())
                .map(|x| x.unwrap())
                .any(|c| *c == 0)
        };

        if !is_first_chars_not_zero(&self.operand_chars) {
            return false;
        }
        if !is_first_chars_not_zero(&self.sum_chars) {
            return false;
        }

        let digitize = |input: &Vec<&str>| {
            input
                .iter()
                .map(|op| {
                    op.chars()
                        .rev()
                        .filter_map(|c| self.char_number_mapping.get(&c))
                        .fold((1u64, 0), |(dig, sum), n| {
                            (dig * 10, dig * u64::from(*n) + sum)
                        })
                })
                .map(|(_, sum)| sum)
                .sum::<u64>()
        };

        let operand_chars_digitized = digitize(&self.operand_chars);
        let sum_chars_digitized = digitize(&self.sum_chars);

        operand_chars_digitized == sum_chars_digitized
    }
}
