#[derive(Debug)]
pub struct Item {
    pub weight: u32,
    pub value: u32,
}

pub fn maximum_value(max_weight: u32, items: &[Item]) -> u32 {
    let col = vec![0u32; items.len() + 1];
    let mut matrix = vec![col.clone(); (max_weight + 1) as usize];

    for i in 1..=items.len() {
        for j in 1..=max_weight as usize {
            if items[i - 1].weight as usize > j {
                matrix[j][i] = matrix[j][i - 1];
            } else {
                matrix[j][i] = std::cmp::max(
                    matrix[j][i - 1],
                    matrix[j - items[i - 1].weight as usize][i - 1] + items[i - 1].value,
                );
            }
        }
    }

    matrix[max_weight as usize][items.len()]
}
