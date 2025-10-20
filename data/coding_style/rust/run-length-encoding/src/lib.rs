pub fn encode(source: &str) -> String {
    let mut res = String::from("");

    if source.is_empty() {
        return res;
    }

    let mut count = 0;
    let mut curr_char = source.chars().next().unwrap();

    for ch in source.chars() {
        if curr_char == ch {
            count += 1;
        } else {
            res.push_str(&match count {
                1 => curr_char.to_string(),
                _ => count.to_string() + &curr_char.to_string(),
            });

            count = 1;
            curr_char = ch;
        }
    }

    res.push_str(&match count {
        1 => curr_char.to_string(),
        _ => count.to_string() + &curr_char.to_string(),
    });

    res
}

pub fn decode(source: &str) -> String {
    source
        .chars()
        .scan(String::from(""), |num, c| {
            let mut res = String::new();

            if c.is_digit(10) {
                num.push(c);
            } else {
                let parsed_num: u32 = num.parse().unwrap_or(1);

                for _ in 0..parsed_num {
                    res.push(c)
                }

                num.clear();
            }

            Some(res)
        })
        .collect()
}
