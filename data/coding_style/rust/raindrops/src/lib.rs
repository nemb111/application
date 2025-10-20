pub fn raindrops(n: u32) -> String {
    let mut result = String::new();

    if n % 3 == 0 {
        result.push_str(&"Pling".to_string());
    }
    if n % 5 == 0 {
        result.push_str(&"Plang".to_string());
    }
    if n % 7 == 0 {
        result.push_str(&"Plong".to_string());
    }

    if result.is_empty() {
        result.push_str(&n.to_string());
    }

    result
}
