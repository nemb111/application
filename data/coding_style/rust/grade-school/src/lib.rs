use std::collections::BTreeMap;
use std::collections::BTreeSet;

#[derive(Default)]
pub struct School {
    roster: BTreeMap<u32, BTreeSet<String>>,
}

impl School {
    pub fn new() -> School {
        Default::default()
    }

    pub fn add(&mut self, grade: u32, student: &str) {
        self.roster
            .entry(grade)
            .or_default()
            .insert(student.to_string());
    }

    pub fn grades(&self) -> Vec<u32> {
        self.roster.keys().cloned().collect()
    }

    // If grade returned an `Option<&Vec<String>>`,
    // the internal implementation would be forced to keep a `Vec<String>` to lend out.
    // By returning an owned vector instead,
    // the internal implementation is free to use whatever it chooses.
    pub fn grade(&self, grade: u32) -> Option<Vec<String>> {
        match self.roster.get(&grade) {
            Some(x) => Some(x.iter().cloned().collect()),
            None => None,
        }
    }
}
