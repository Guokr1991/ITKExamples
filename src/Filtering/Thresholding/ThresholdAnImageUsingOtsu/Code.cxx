/*=========================================================================
 *
 *  Copyright Insight Software Consortium
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *         http://www.apache.org/licenses/LICENSE-2.0.txt
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 *=========================================================================*/

#include "itkImageFileReader.h"
#include "itkOtsuMultipleThresholdsImageFilter.h"
#include "itkRescaleIntensityImageFilter.h"
#include "itkImageFileWriter.h"

int main( int argc, char* argv[] )
{
  if( argc < 6 )
    {
    std::cerr << "Usage: " << std::endl;
    std::cerr << argv[0] << std::endl;
    std::cerr << " <InputImage> <OutputImage> <NumberOfBins>";
    std::cerr << " <NumberOfThresholds> <LabelOffset>"
              << std::endl;
    return EXIT_FAILURE;
    }

  const unsigned int Dimension = 2;
  typedef  unsigned char  PixelType;
  typedef  itk::SizeValueType  SizeType;

  const char * InputImage = argv[1];
  const char * OutputImage = argv[2];

  const SizeType NumberOfHistogramBins = static_cast<SizeType>(atoi( argv[3] ) );
  const SizeType NumberOfThresholds = static_cast<SizeType>(atoi( argv[4] ) );
  const PixelType LabelOffset = static_cast<PixelType>(atoi( argv[5] ) );

  typedef itk::Image< PixelType, Dimension >  ImageType;

  typedef itk::ImageFileReader< ImageType > ReaderType;
  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName( InputImage );

  typedef itk::OtsuMultipleThresholdsImageFilter< ImageType, ImageType >
    FilterType;
  FilterType::Pointer filter = FilterType::New();
  filter->SetInput( reader->GetOutput() );
  filter->SetNumberOfHistogramBins( NumberOfHistogramBins );
  filter->SetNumberOfThresholds( NumberOfThresholds );
  filter->SetLabelOffset( LabelOffset );

  FilterType::ThresholdVectorType thresholds = filter->GetThresholds();

  std::cout << "Thresholds:" << std::endl;

  for( size_t i = 0; i < thresholds.size(); i++ )
    {
    std:: cout << thresholds[i] << std::endl;
    }

  std::cout << std::endl;

  typedef itk::RescaleIntensityImageFilter< ImageType, ImageType > RescaleType;
  RescaleType::Pointer rescaler = RescaleType::New();
  rescaler->SetInput( filter->GetOutput() );
  rescaler->SetOutputMinimum( 0 );
  rescaler->SetOutputMaximum( 255 );

  typedef itk::ImageFileWriter< ImageType >  WriterType;
  WriterType::Pointer writer = WriterType::New();
  writer->SetFileName( OutputImage );
  writer->SetInput( rescaler->GetOutput() );

  try
    {
    writer->Update();
    }
  catch( itk::ExceptionObject & e )
    {
    std::cerr << "Error: " << e << std::endl;
    return EXIT_FAILURE;
    }

  return EXIT_SUCCESS;
}
