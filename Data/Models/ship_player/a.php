
public function addBill($bill_details)
{
	$name = $bill_details['name'];
	$fixed = $bill_details['fixed']?'1':'0';
	$common = $bill_details['common']?'1':'0';
	$user_specific = $bill_details['user_specific']?'1':'0';
	$fixed_amount = $bill_details['fixed_amount'];
	$society_id = $bill_details['society_id'];
	$query = "INSERT into bill_category(name,fixed,common,user_specific,fixed_amount,society_id) VALUES('$name','"	
}