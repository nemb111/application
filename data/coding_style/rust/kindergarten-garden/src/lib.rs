use std::collections::HashMap;
use std::iter::zip;
use std::sync::LazyLock;
use std::vec;

static STUDENT_NAMES: LazyLock<HashMap<&str, u32>> = LazyLock::new(|| {
    HashMap::from([
        ("alice", 0u32),
        ("bob", 1u32),
        ("charlie", 2u32),
        ("david", 3u32),
        ("eve", 4u32),
        ("fred", 5u32),
        ("ginny", 6u32),
        ("harriet", 7u32),
        ("ileana", 8u32),
        ("joseph", 9u32),
        ("kincaid", 10u32),
        ("larry", 11u32),
    ])
});

static PLANT_NAMES: LazyLock<HashMap<u8, &'static str>> = LazyLock::new(|| {
    HashMap::from([
        (b'G', "grass"),
        (b'C', "clover"),
        (b'R', "radishes"),
        (b'V', "violets"),
    ])
});

pub fn plants(diagram: &str, student: &str) -> Vec<&'static str> {
    let vec_of_lines: Vec<_> = diagram
        .lines()
        .map(|l| l.as_bytes().chunks_exact(2))
        .collect();

    let zipped_vec: Vec<_> = zip(vec_of_lines[0].clone(), vec_of_lines[1].clone()).collect();

    if let Some(&index) = STUDENT_NAMES.get(student.to_ascii_lowercase().as_str()) {
        let (first_pair, second_pair) = zipped_vec[index as usize];

        vec![
            PLANT_NAMES.get(&first_pair[0]).unwrap(),
            PLANT_NAMES.get(&first_pair[1]).unwrap(),
            PLANT_NAMES.get(&second_pair[0]).unwrap(),
            PLANT_NAMES.get(&second_pair[1]).unwrap(),
        ]
    } else {
        panic!("Student not found");
    }
}
