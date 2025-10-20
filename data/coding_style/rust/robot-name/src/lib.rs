pub struct Robot {
    name: String,
}

impl Robot {
    pub fn new() -> Self {
        let name = create_robot_name();

        Robot { name }
    }

    pub fn name(&self) -> &str {
        &self.name
    }

    pub fn reset_name(&mut self) {
        let name = create_robot_name();

        self.name = name;
    }
}

use std::cell::RefCell;
use std::collections::HashSet;
thread_local! {
    pub static ALL_ROBOT_NAMES: RefCell<HashSet<String>> = RefCell::new(HashSet::new());
}

use rand::distributions::Uniform;
use rand::{thread_rng, Rng};

fn rand_robot_name() -> String {
    let mut rng = thread_rng();
    format!(
        "{}{}{}{}{}",
        char::from(rng.sample(Uniform::new(b'A', b'Z' + 1))),
        char::from(rng.sample(Uniform::new(b'A', b'Z' + 1))),
        rng.sample(Uniform::new(0, 10)),
        rng.sample(Uniform::new(0, 10)),
        rng.sample(Uniform::new(0, 10))
    )
}

fn create_robot_name() -> String {
    let mut name = rand_robot_name();
    ALL_ROBOT_NAMES.with(|all_robot_names| {
        let mut tmp = all_robot_names.borrow_mut();

        while !tmp.insert(name.clone()) {
            name = rand_robot_name();
        }
    });

    name
}
