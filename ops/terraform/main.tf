provider "aws" {
  region = "${var.aws_region}"
}

# test example
resource "aws_instance" "skale_node" {
  instance_type = "t2.micro"
  ami = "${lookup(var.aws_amis, var.aws_region)}"
  tags {
    Name = "skale_node_${count.index}"
  }
  count = 5
}

resource "local_file" "out_ips" {
  filename = "../../nodes_data/out_ips.json"
  content = "${jsonencode(aws_instance.skale_node.*.public_ip)}"

}