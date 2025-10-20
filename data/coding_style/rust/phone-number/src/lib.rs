pub fn number(user_number: &str) -> Option<String> {
   let stripped_num = user_number
      .chars()
      .filter(|c| c.is_ascii_digit())
      .collect::<Vec<_>>();

   if stripped_num.len() == 10
      && stripped_num[0].to_digit(10).unwrap() > 1
      && stripped_num[3].to_digit(10).unwrap() > 1
   {
      return Some(stripped_num.into_iter().collect());
   } else if stripped_num.len() == 11
      && stripped_num[0] == '1'
      && stripped_num[1].to_digit(10).unwrap() > 1
      && stripped_num[4].to_digit(10).unwrap() > 1
   {
      return Some(stripped_num.into_iter().skip(1).collect());
   }

   None
}
