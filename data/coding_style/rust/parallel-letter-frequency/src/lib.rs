use std::collections::HashMap;
use std::thread;

pub fn frequency(input: &[&str], worker_count: usize) -> HashMap<char, usize> {
    let mut res = HashMap::new();

    if input.is_empty() {
        return res;
    }

    let mut worker_count = worker_count;

    let num_chunks = if input.len() < worker_count {
        worker_count = 1;
        input.len()
    } else {
        (input.len() as f64 / worker_count as f64).ceil() as usize
    };

    
    let chnk = input.chunks(num_chunks);
    let th_handles : Vec<_> = chnk.clone().take(worker_count - 1).map(
        |sl|{
            let th_string = sl.join("");

            thread::spawn(move || char_frequency(&th_string))
        }
    ).collect();


    if let Some(sl) = chnk.last() {
        res = char_frequency(&sl.join(""));
    }

    th_handles.into_iter().for_each(|hd| {
        let hm = hd.join().unwrap();
        for (key, value) in hm {
            res.entry(key)
                .and_modify(|va| *va += value)
                .or_insert(value);
        }
    });

    res
}

fn char_frequency(input: &str) -> HashMap<char, usize> {
    let mut res = HashMap::with_capacity(128usize);

    input
        .chars()
        .filter(|c| c.is_alphabetic())
        .flat_map(|c| c.to_lowercase())
        .for_each(|c| {
            let counter = res.entry(c).or_insert(0);
            *counter += 1;
        });

    res
}
