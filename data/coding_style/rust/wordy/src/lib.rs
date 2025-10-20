pub struct WordProblem;

pub fn answer(command: &str) -> Option<i32> {
    let trim_cmd = command.trim_end_matches(|c| c == '?');

    if command.len() == trim_cmd.len() {
        return None;
    }

    let mut split_cmd = trim_cmd.split_whitespace();

    if !split_cmd
        .by_ref()
        .take(2)
        .eq(["What", "is"].iter().copied())
    {
        return None;
    }

    let mut first_operand = split_cmd.by_ref().next()?.parse::<i32>().ok()?;
    while let Some(i) = split_cmd.next() {
        let mut input = i.to_string();
        if i == "multiplied" || i == "divided" {
            input = input + " " + split_cmd.next()?
        };

        let operator = get_operator(&input)?;
        let sec_operand = split_cmd.next()?.parse::<i32>().ok()?;

        first_operand = operator(first_operand, sec_operand);
    }

    Some(first_operand)
}

fn get_operator(operator: &str) -> Option<impl Fn(i32, i32) -> i32> {
    Some(match operator {
        "plus" => std::ops::Add::add,
        "minus" => std::ops::Sub::sub,
        "multiplied by" => std::ops::Mul::mul,
        "divided by" => std::ops::Div::div,
        _ => return None,
    })
}
