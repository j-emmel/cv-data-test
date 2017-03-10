#!/usr/bin/ruby

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
    FeretSubject.new(subject_xml.at_xpath('@id'),
                     subject_xml.at_xpath('//Gender/@value'),
                     subject_xml.at_xpath('//YOB/@value'),
                     subject_xml.at_xpath('//Race/@value'))
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
    ImageData.new(subject_xml.at_xpath('@id'),
                  img_path,
                  self.eql_ignore_case?('Yes', face_xml.at_xpath('//Pose/@name')),
                  self.eql_ignore_case?('Yes', face_xml.at_xpath('//Wearing/@glasses')),
                  self.eql_ignore_case?('Yes', hair_xml.at_xpath('@beard')),
                  self.eql_ignore_case?('Yes', hair_xml.at_xpath('@mustache')))
  end

  def self.eql_ignore_case?(string1, string2)
    string1.casecmp(string2) == 0
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

def process_dvd(feret_path, dvd_num)
  feret_xml_path = "#{feret_path}/dvd#{dvd_num}/data/ground_truths/xml"

  # Iterate over the directories for each subject
  Dir.foreach(feret_xml_path) do |subject_xml_dir|
    # Ignore current and parent directories
    next if subject_xml_dir == '.' || subject_xml_dir == '..'

    puts get_demographics_from_file(feret_xml_path, subject_xml_dir)

    # Iterate over the files in a single subject's directory
    subject_xml_full_path = "#{feret_xml_path}/#{subject_xml_dir}"
    Dir.foreach(subject_xml_full_path) do |xml_file_name|
      # Ignore non-XML files and the subject information file
      next if (File.extname(xml_file_name) != '.xml') || (File.basename(xml_file_name, '.xml') == subject_xml_dir)

      xml_file_path = "#{subject_xml_full_path}/#{xml_file_name}"
      puts get_image_data_from_file(xml_file_path)
    end
  end
end


# ####
# Selection Script
# ####

if ARGV.size < 1
  puts "Usage: #{$PROGRAM_NAME} feretPath"
  puts '  feretPath: path to FERET directory'
  exit -1
end

feret_path = ARGV[0].dup
feret_path.chop! if feret_path[-1] == '/'

unless Dir.exist?(feret_path)
  puts "Error: Could not find directory #{feret_path}"
  exit -1
end

process_dvd(feret_path, 1)
#process_dvd(feret_path, 2)
