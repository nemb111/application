use std::collections::VecDeque;
use std::convert::TryFrom;

pub struct RailFence {
    rails: u32,
}

impl RailFence {
    pub fn new(rails: u32) -> RailFence {
        Self { rails }
    }

    pub fn encode(&self, text: &str) -> String {
        let mut string_rails = vec![String::new(); usize::try_from(self.rails).unwrap()];

        let it = (0..self.rails).chain((1..self.rails - 1).rev()).cycle();
        for el in it.zip(text.chars()) {
            string_rails[usize::try_from(el.0).unwrap()].push(el.1)
        }

        string_rails.join("")
    }

    pub fn decode(&self, cipher: &str) -> String {
        let mut string_rails: Vec<VecDeque<char>> =
            vec![VecDeque::new(); usize::try_from(self.rails).unwrap()];
        let cipher_it = &mut cipher.chars();
        let it = (0..self.rails)
            .chain((1..self.rails - 1).rev())
            .cycle()
            .take(cipher.len());

        for i in 0..self.rails {
            for _ in 0..it.clone().filter(|el| *el == i).count() {
                string_rails[i as usize].push_back(cipher_it.next().unwrap())
            }
        }

        let mut res: String = String::new();

        for i in it {
            res.push(string_rails[i as usize].pop_front().unwrap())
        }

        res
    }
}
