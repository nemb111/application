pub fn brackets_are_balanced(string: &str) -> bool {
    let mut stack = Vec::new();
    let mut passed = true;

    string.chars().for_each(|c| match c {
        '(' | '[' | '{' => stack.push(c),
        ')' => match stack.last() {
            Some(&'(') => {
                stack.pop();
            }
            _ => passed = false,
        },
        ']' => match stack.last() {
            Some(&'[') => {
                stack.pop();
            }
            _ => passed = false,
        },
        '}' => match stack.last() {
            Some(&'{') => {
                stack.pop();
            }
            _ => passed = false,
        },
        _ => {}
    });

    stack.is_empty() && passed
}
