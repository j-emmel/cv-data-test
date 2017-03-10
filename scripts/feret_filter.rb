#!/usr/bin/ruby

require 'optparse'

require 'rubygems'
require 'nokogiri'


# ####
# Class Definitions
# ####

class FeretSubject
  attr_accessor :id, :gender, :birth_year, :race

  def initialize(id, gender, birth_year, race)
    @id = id
    @gender = gender
    @birth_year = birth_year
    @race = race
  end

  def to_s
    "{ VizSubject | id: #{@id}, gender: #{@gender}, birth_year: #{@birth_year}, race: #{@race} }"
  end

  def self.from_xml(subject_xml)
    FeretSubject.new(subject_xml.at_xpath('@id').to_s,
                     subject_xml.at_xpath('//Gender/@value').to_s,
                     subject_xml.at_xpath('//YOB/@value').to_s,
                     subject_xml.at_xpath('//Race/@value').to_s)
  end
end

class ImageData
  attr_accessor :id, :path, :pose, :glasses, :beard, :mustache

  def initialize(id, path, pose, glasses, beard, mustache)
    @id = id
    @path = path
    @pose = pose
    @glasses = glasses
    @beard = beard
    @mustache = mustache
  end

  def to_s
    "{ ImageData | id: #{@id}, pose: #{@pose}, glasses: #{@glasses}, beard: #{@beard}, mustache: #{@mustache}, path: #{@path} }"
  end

  def self.from_xml(subject_xml, img_path)
    face_xml = subject_xml.at_xpath('//Application//Face')
    hair_xml = face_xml.at_xpath('//Hair')
    ImageData.new(subject_xml.at_xpath('@id').to_s,
                  img_path.to_s,
                  face_xml.at_xpath('//Pose/@name').to_s,
                  self.eql_ignore_case?('Yes', face_xml.at_xpath('//Wearing/@glasses')),
                  self.eql_ignore_case?('Yes', hair_xml.at_xpath('@beard')),
                  self.eql_ignore_case?('Yes', hair_xml.at_xpath('@mustache')))
  end

  def self.eql_ignore_case?(string1, string2)
    string1.casecmp(string2) == 0
  end
end

class SubjectFilters
  attr_writer :genders, :poses

  def initialize
    @inclusions = %i(any yes no)
    @genders = %w(Male Female)
    @poses = %w(fa fb pl hl ql pr hr qr ra rb rc rd re)
    @glasses = :any
    @beard = :any
    @mustache = :any
  end

  def to_s
    "{ SubjectFilters | genders: #{@genders}, poses: #{@poses}, glasses: #{@glasses}, beard: #{@beard}, mustache: #{@mustache} }"
  end

  def glasses=(glasses)
    return unless @inclusions.include?(glasses.to_sym)
    @glasses = glasses.to_sym
  end

  def mustache=(mustache)
    return unless @inclusions.include?(mustache.to_sym)
    @mustache = mustache.to_sym
  end

  def beard=(beard)
    return unless @inclusions.include?(beard.to_sym)
    @beard = beard.to_sym
  end

  def self.do_accept?(inclusion, value)
    (inclusion == :any) || (inclusion == :no && value == false) || (inclusion == :yes && value == true)
  end

  def accept_subject?(subject)
    @genders.include?(subject.gender)
  end

  def accept_image?(image_data)
    (@poses.include?(image_data.pose) &&
        self.class.do_accept?(@glasses, image_data.glasses) &&
        self.class.do_accept?(@beard, image_data.beard) &&
        self.class.do_accept?(@mustache, image_data.mustache))
  end
end


# ####
# Function Definitions
# ####

# Given the path to the Color FERET xml ground truths directory and a subject ID, locates the subject's demographic
# file and extracts data from it into a FeretSubject object.
def get_demographics_from_file(feret_xml_path, subject_id)
  subject_xml_full_path = "#{feret_xml_path}/#{subject_id}"
  subject_doc = Nokogiri::XML(File.open("#{subject_xml_full_path}/#{subject_id}.xml"))

  FeretSubject.from_xml(subject_doc.at_xpath('//Subjects//Subject'))
end

# Given the path to a Color FERET image's ground truth XML file, loads data from the file into an ImageData object.
def get_image_data_from_file(xml_file_path)
  document = Nokogiri::XML(File.open(xml_file_path))
  recording = document.at_xpath('//Recordings//Recording')
  img_path = "dvd#{recording.at_xpath('//URL/@root').to_s[-1]}/#{recording.at_xpath('//URL/@relative')}"

  ImageData.from_xml(recording.at_xpath('//Subject'), img_path)
end

def process_dvd(feret_path, dvd_num, filters, output_file)
  feret_xml_rel = "dvd#{dvd_num}/data/ground_truths/xml"
  feret_xml_path = "#{feret_path}/#{feret_xml_rel}"

  # Iterate over the directories for each subject
  Dir.foreach(feret_xml_path) do |subject_xml_dir|
    # Ignore current and parent directories
    next if subject_xml_dir == '.' || subject_xml_dir == '..' || !Dir.exist?("#{feret_xml_path}/#{subject_xml_dir}")

    # Filter out this image subject based on demographic information
    feret_subject = get_demographics_from_file(feret_xml_path, subject_xml_dir)
    next unless filters.accept_subject?(feret_subject)
    #puts feret_subject

    # Iterate over the files in a single subject's directory
    subject_xml_full_path = "#{feret_xml_path}/#{subject_xml_dir}"
    Dir.foreach(subject_xml_full_path) do |xml_file_name|
      # Ignore non-XML files and the subject information file
      next if (File.extname(xml_file_name) != '.xml') || (File.basename(xml_file_name, '.xml') == subject_xml_dir)

      feret_image_data = get_image_data_from_file("#{subject_xml_full_path}/#{xml_file_name}")
      next unless filters.accept_image?(feret_image_data)

      # Write (XML Path, Image Path) to output file
      output_file.puts "#{feret_xml_rel}/#{subject_xml_dir}/#{xml_file_name},#{feret_image_data.path}"
      #puts feret_image_data
    end
  end
end


# ####
# Selection Script
# ####

# Process command line options to determine what data to filter out
filters = SubjectFilters.new
output_name = 'filtered_feret.csv'
opts = OptionParser.new
opts.banner = "Usage: #{$PROGRAM_NAME} feretPath [options]"


opts.on('-d', '--dest PATH', String,
        "Path to output file, overwritten with results (default: #{output_name})") do |destination|
  output_name = destination
end

opts.on('-g', '--genders GENDERS', Array,
        'Comma-separated list of genders to include (Male Female)') do |genders|
  filters.genders = genders
end

opts.on('-p', '--poses POSES', Array,
        'Comma-separated list of poses to include (fa fb pl hl ql pr hr qr ra rb rc rd re)') do |poses|
  filters.poses = poses
end

opts.on('-e', '--glasses INCLUDE', String,
        'How images should be included based on glasses (any, yes, no)') do |include|
  filters.glasses = include
end

opts.on('-b', '--beard INCLUDE', String,
        'How images should be included based on beard (any, yes, no)') do |include|
  filters.beard = include
end

opts.on('-m', '--mustache INCLUDE', String,
        'How images should be included based on glasses (any, yes, no)') do |include|
  filters.mustache = include
end
opts.parse!(ARGV)

# After parsing options, what's left should be the path to the Color FERET directory
if ARGV.empty?
  puts opts
  exit -1
end

feret_path = ARGV.pop
feret_path.chop! if feret_path[-1] == '/'

unless Dir.exist?(feret_path)
  puts "Error: Could not find directory #{feret_path}"
  exit -1
end

File.open(output_name, 'w') do |output_file|
  process_dvd(feret_path, 1, filters, output_file)
  process_dvd(feret_path, 2, filters, output_file)
end
