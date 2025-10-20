// For want of a nail the shoe was lost.
// For want of a shoe the horse was lost.
// For want of a horse the rider was lost.
// For want of a rider the message was lost.
// For want of a message the battle was lost.
// For want of a battle the kingdom was lost.
// And all for the want of a nail.

pub fn build_proverb(list: &[&str]) -> String {
    if list.is_empty() {
        return String::new();
    }

    let mut it = list
        .iter()
        .zip(list.iter().cloned().skip(1))
        .map(|(x, y)| format!("For want of a {0} the {1} was lost.\n", x, y))
        .collect::<Vec<String>>()
        .join("");

    it.push_str(&format!("And all for the want of a {}.", list[0]));
    it
}
