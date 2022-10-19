import React from 'react';
import PageWrapper from 'components/internal/PageWrapper';
import NumberDisplay from 'components/NumberDisplay';

const Page: React.FC = function () {
  return (
   <PageWrapper title="DHT Status">
      <NumberDisplay label="Temperature" dataID="temperature" unit="C" />
      <NumberDisplay label="Humidity" dataID="humidity" unit="%" />

      <NumberDisplay
        label="temperature After 1m"
        dataID='temperature'
        dataDispID='temperature-inf'
        action='inference'
        unit='C'
      />
      <NumberDisplay
        label="Humidity After 1m"
        dataID='humidity'
        dataDispID='humidity-inf'
        action='inference'
        unit='%'
      />
    </PageWrapper>
  );
};

export default Page;
