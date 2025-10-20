const fn num_to_str(n: u32) -> &'static str {
    match n {
        0 => "No",
        1 => "One",
        2 => "Two",
        3 => "Three",
        4 => "Four",
        5 => "Five",
        6 => "Six",
        7 => "Seven",
        8 => "Eight",
        9 => "Nine",
        10 => "Ten",
        _ => unreachable!(),
    }
}

fn generate_verse(number: u32) -> String {
    let opt_s = if number == 1 { "" } else { "s" };
    let next_opt_s = if number - 1 == 1 { "" } else { "s" };
    let number_str = num_to_str(number);
    let next_number_str = num_to_str(number - 1).to_lowercase();

    format!(
        "{number_str} green bottle{opt_s} hanging on the wall,\n\
         {number_str} green bottle{opt_s} hanging on the wall,\n\
         And if one green bottle should accidentally fall,\n\
         There'll be {next_number_str} green bottle{next_opt_s} hanging on the wall.\n",
    )
}

pub fn recite(start_bottles: u32, take_down: u32) -> String {
    let mut res = String::new();
    for bottle in (start_bottles - take_down + 1..=start_bottles).rev() {
        res.push_str(&(generate_verse(bottle) + "\n"));
    }

    res
}
