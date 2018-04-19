variable "aws_region" {
  description = "The AWS region to create things in."
  default     = "us-east-2"
}

# Ubuntu 16.04 LTS
variable "aws_amis" {
  type = "map"
  default = {
    "us-east-2" = "ami-916f59f4"
  }
}