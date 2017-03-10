# FERET Filtering Script

For splitting Color FERET into more manageable pieces.

## Running

You'll need these:

* Ruby
* [Rubygems](https://rubygems.org/pages/download) 
* [Nokogiri](http://www.nokogiri.org/tutorials/installing_nokogiri.html)

Then, you can run the script as

```shell
ruby feret_filter.rb feretPath [options]
```

`feretPath` is the path to the Color FERET directory. The `options` you can provide indicate what data you want and 
where it should go. By default, all images are included. (That is, the full gender and pose lists are set, and the other
filters are 'any.')

| Option                  | Value                                                                             |
| ---                     |----                                                                               |
| -d, --dest PATH         | Path to output file, overwritten with results (default: filtered_feret.csv)       |
| -g, --genders GENDERS   | Comma-separated list of genders to include (Male Female)                          |
| -p, --poses POSES       | Comma-separated list of poses to include (fa fb pl hl ql pr hr qr ra rb rc rd re) |
| -e, --glasses INCLUDE   | How images should be included based on glasses (any/yes/no)                       |
| -b, --beard INCLUDE     | How images should be included based on beards (any/yes/no)                        |
| -m, --mustache INCLUDE  | How images should be included based on mustaches (any/yes/no)                     |

As an example, you could run this to get a list of both male and female subjects with glasses, writing the output to 
_feretGlasses.csv_ in the current directory.

```shell
ruby feret_filter.rb ../../colorferet --dest feretGlasses.csv --genders Male,Female --glasses yes
```

## Output Format

The output file is a two-column CSV with one row per accepted image, where the column values are

1. the path to an image's ground truth XML file
2. the path to the compressed image file
